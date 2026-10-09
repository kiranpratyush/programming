#!/usr/bin/env bash
# M2: hA1 -- R1 -- R2 -- R3 -- hB1 with static routes. Idempotent: tears down first.
set -euo pipefail
cd "$(dirname "$0")"

./m2-teardown.sh

NS="ns-ha1 ns-hb1 ns-r1 ns-r2 ns-r3"
ROUTERS="ns-r1 ns-r2 ns-r3"

# 1. namespaces
for ns in $NS; do sudo ip netns add "$ns"; done

# 2. veth pairs, each end moved into its namespace
link() { # <nsA> <ifA> <nsB> <ifB>
  sudo ip link add "$2" netns "$1" type veth peer name "$4" netns "$3"
}
link ns-ha1 ns-ha1-eth0 ns-r1 ns-r1-eth0
link ns-r1  ns-r1-eth1  ns-r2 ns-r2-eth0
link ns-r2  ns-r2-eth1  ns-r3 ns-r3-eth0
link ns-r3  ns-r3-eth1  ns-hb1 ns-hb1-eth0

# 3. addresses
addr() { sudo ip -n "$1" addr add "$2" dev "$3"; }
addr ns-ha1 10.10.0.2/26  ns-ha1-eth0
addr ns-r1  10.10.0.1/26  ns-r1-eth0
addr ns-r1  10.10.0.81/30 ns-r1-eth1
addr ns-r2  10.10.0.82/30 ns-r2-eth0
addr ns-r2  10.10.0.85/30 ns-r2-eth1
addr ns-r3  10.10.0.86/30 ns-r3-eth0
addr ns-r3  10.10.0.65/28 ns-r3-eth1
addr ns-hb1 10.10.0.66/28 ns-hb1-eth0

# 4. links up
for ns in $NS; do sudo ip -n "$ns" link set lo up; done
for pair in ns-ha1:ns-ha1-eth0 ns-r1:ns-r1-eth0 ns-r1:ns-r1-eth1 \
            ns-r2:ns-r2-eth0 ns-r2:ns-r2-eth1 ns-r3:ns-r3-eth0 \
            ns-r3:ns-r3-eth1 ns-hb1:ns-hb1-eth0; do
  sudo ip -n "${pair%%:*}" link set "${pair##*:}" up
done

# 5. forwarding on every router
for r in $ROUTERS; do sudo ip netns exec "$r" sysctl -qw net.ipv4.ip_forward=1; done

# 6. routes (hosts: default; routers: every non-connected subnet)
sudo ip -n ns-ha1 route add default via 10.10.0.1
sudo ip -n ns-hb1 route add default via 10.10.0.65

# R1: LAN-B and link R2-R3 are via R2
sudo ip -n ns-r1 route add 10.10.0.64/28 via 10.10.0.82
sudo ip -n ns-r1 route add 10.10.0.84/30 via 10.10.0.82
# R2: LAN-A and link R1-R2 side via R1, LAN-B via R3
sudo ip -n ns-r2 route add 10.10.0.0/26  via 10.10.0.81
sudo ip -n ns-r2 route add 10.10.0.64/28 via 10.10.0.86
# R3: LAN-A and link R1-R2 are via R2
sudo ip -n ns-r3 route add 10.10.0.0/26  via 10.10.0.85
sudo ip -n ns-r3 route add 10.10.0.80/30 via 10.10.0.85

# 7. quick check
sudo ip netns exec ns-ha1 ping -c 2 -W 2 10.10.0.66
echo "Proof:   sudo ip netns exec ns-ha1 traceroute -n 10.10.0.66"
echo "Break:   sudo ip -n ns-r2 route del 10.10.0.0/26   # remove only R2's return route"
