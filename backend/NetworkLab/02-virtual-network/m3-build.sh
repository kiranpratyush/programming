#!/usr/bin/env bash
# M3: two LANs with switches (bridges).
#   LAN-A: ha1 ha2 ha3 -> br-a (inside ns-r1) -> R1 -- R2 -- R3 <- br-b (inside ns-r3) <- hb1 hb2 :LAN-B
set -euo pipefail
cd "$(dirname "$0")"

./m3-teardown.sh

HOSTS_A="ns-ha1:10.10.0.2/26 ns-ha2:10.10.0.3/26 ns-ha3:10.10.0.4/26"
HOSTS_B="ns-hb1:10.10.0.66/28 ns-hb2:10.10.0.67/28"
ROUTERS="ns-r1 ns-r2 ns-r3"

for ns in ns-r1 ns-r2 ns-r3 ns-ha1 ns-ha2 ns-ha3 ns-hb1 ns-hb2; do
  sudo ip netns add "$ns"
  sudo ip -n "$ns" link set lo up
done

# attach_lan <bridge> <router-ns> <router-ip/prefix> <gateway-ip> "<host:ip/prefix> ..."
# The bridge lives in the router namespace. Each host gets a veth whose far end
# is a bridge port; the router's own LAN interface is also a bridge port.
attach_lan() {
  local br=$1 rns=$2 rip=$3 gw=$4 hosts=$5 h ns ip short
  sudo ip -n "$rns" link add "$br" type bridge
  sudo ip -n "$rns" link set "$br" up

  # router <-> bridge: ns-rX-lan holds the router IP, <br>-rp is the bridge port
  sudo ip -n "$rns" link add "$rns-lan" type veth peer name "$br-rp"
  sudo ip -n "$rns" link set "$br-rp" master "$br" up
  sudo ip -n "$rns" addr add "$rip" dev "$rns-lan"
  sudo ip -n "$rns" link set "$rns-lan" up

  for h in $hosts; do
    ns=${h%%:*}; ip=${h##*:}; short=${ns#ns-}
    sudo ip link add "$ns-eth0" netns "$ns" type veth peer name "$br-$short" netns "$rns"
    sudo ip -n "$rns" link set "$br-$short" master "$br" up
    sudo ip -n "$ns" addr add "$ip" dev "$ns-eth0"
    sudo ip -n "$ns" link set "$ns-eth0" up
    sudo ip -n "$ns" route add default via "$gw"
  done
}

attach_lan br-a ns-r1 10.10.0.1/26  10.10.0.1  "$HOSTS_A"
attach_lan br-b ns-r3 10.10.0.65/28 10.10.0.65 "$HOSTS_B"

# router-to-router links
link() { # <nsA> <ifA> <ipA> <nsB> <ifB> <ipB>
  sudo ip link add "$2" netns "$1" type veth peer name "$5" netns "$4"
  sudo ip -n "$1" addr add "$3" dev "$2"; sudo ip -n "$1" link set "$2" up
  sudo ip -n "$4" addr add "$6" dev "$5"; sudo ip -n "$4" link set "$5" up
}
link ns-r1 ns-r1-eth1 10.10.0.81/30 ns-r2 ns-r2-eth0 10.10.0.82/30
link ns-r2 ns-r2-eth1 10.10.0.85/30 ns-r3 ns-r3-eth0 10.10.0.86/30

for r in $ROUTERS; do sudo ip netns exec "$r" sysctl -qw net.ipv4.ip_forward=1; done

sudo ip -n ns-r1 route add 10.10.0.64/28 via 10.10.0.82
sudo ip -n ns-r1 route add 10.10.0.84/30 via 10.10.0.82
sudo ip -n ns-r2 route add 10.10.0.0/26  via 10.10.0.81
sudo ip -n ns-r2 route add 10.10.0.64/28 via 10.10.0.86
sudo ip -n ns-r3 route add 10.10.0.0/26  via 10.10.0.85
sudo ip -n ns-r3 route add 10.10.0.80/30 via 10.10.0.85

# checks: same-LAN ping (never touches R1), cross-LAN ping, then the switch's MAC table
sudo ip netns exec ns-ha1 ping -c 2 -W 2 10.10.0.3
sudo ip netns exec ns-ha1 ping -c 2 -W 2 10.10.0.66
sudo ip netns exec ns-r1 bridge fdb show br br-a
cat <<'EOF'

Proof ideas:
  sudo ip netns exec ns-ha3 tcpdump -n -e -i ns-ha3-eth0 arp     # then: sudo ip netns exec ns-ha1 ping -c1 10.10.0.3
  sudo ip netns exec ns-r1 tcpdump -n -i ns-r1-lan icmp           # silent during ha1 -> ha2 ping
EOF
