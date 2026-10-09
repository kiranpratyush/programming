#!/usr/bin/env bash
# M1 build: ns-ha1 -- ns-r1 -- ns-hb1
# Run with: sudo ./build.sh
# Safe to run twice: it deletes the old namespaces first.
set -euo pipefail

cd "$(dirname "$0")"
./teardown.sh

# Namespaces
ip netns add ns-ha1
ip netns add ns-r1
ip netns add ns-hb1

# Veth pairs, then move each end into its namespace
ip link add ns-ha1-eth0 type veth peer name ns-r1-eth0
ip link add ns-hb1-eth0 type veth peer name ns-r1-eth1
ip link set ns-ha1-eth0 netns ns-ha1
ip link set ns-hb1-eth0 netns ns-hb1
ip link set ns-r1-eth0  netns ns-r1
ip link set ns-r1-eth1  netns ns-r1

# Addresses
ip -n ns-ha1 addr add 10.10.0.2/26  dev ns-ha1-eth0
ip -n ns-hb1 addr add 10.10.0.66/28 dev ns-hb1-eth0
ip -n ns-r1  addr add 10.10.0.1/26  dev ns-r1-eth0
ip -n ns-r1  addr add 10.10.0.65/28 dev ns-r1-eth1

# Links up
for ns in ns-ha1 ns-hb1 ns-r1; do
    ip -n "$ns" link set lo up
done
ip -n ns-ha1 link set ns-ha1-eth0 up
ip -n ns-hb1 link set ns-hb1-eth0 up
ip -n ns-r1  link set ns-r1-eth0 up
ip -n ns-r1  link set ns-r1-eth1 up

# Default routes on the hosts (the gateway is a bare IP, no prefix)
ip -n ns-ha1 route add default via 10.10.0.1
ip -n ns-hb1 route add default via 10.10.0.65

# Forwarding is OFF by default. Turn it on for the "working" test:
#   sudo ip netns exec ns-r1 sysctl -w net.ipv4.ip_forward=1
echo "ip_forward in ns-r1: $(ip netns exec ns-r1 sysctl -n net.ipv4.ip_forward)"
echo "Built. Test: sudo ip netns exec ns-ha1 ping -c 3 -W 2 10.10.0.66"
