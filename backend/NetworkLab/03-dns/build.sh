#!/usr/bin/env bash
# M5 network + DNS subnet: ns-auth, ns-res and ns-auth-b hang off R4 through the bridge br-dns.
#
#   LAN-A -- R1 -- R2 -- R3 -- LAN-B
#             \    |    /
#              `-- R4 --'          (R4 links to R1, R2 and R3)
#                  |
#                  |\
#                  +-- br-dns -- ns-auth, ns-res, ns-auth-b   (10.10.0.112/29)
#                 edge
#                  |
#            ubuntu (root ns) -- eth0 -- Windows -- home router -- internet
set -euo pipefail
cd "$(dirname "$0")"

./teardown.sh

# ---- subnets (from the M0 design) ----
A=10.10.0.0/26      B=10.10.0.64/28
L12=10.10.0.80/30   L23=10.10.0.84/30   L14=10.10.0.88/30
L24=10.10.0.92/30   L34=10.10.0.96/30   L4E=10.10.0.100/30  LEU=10.10.0.104/30
DNS=10.10.0.112/29

for ns in ns-r1 ns-r2 ns-r3 ns-r4 ns-edge ns-ha1 ns-ha2 ns-ha3 ns-hb1 ns-hb2 ns-auth ns-res ns-auth-b; do
  sudo ip netns add "$ns"
  sudo ip -n "$ns" link set lo up
done

# ---- resolv.conf per host: "ip netns exec <ns>" bind-mounts /etc/netns/<ns>/resolv.conf ----
# Every host asks ns-res. ns-auth, ns-res and ns-auth-b do not need one.
for ns in ns-ha1 ns-ha2 ns-ha3 ns-hb1 ns-hb2; do
  sudo mkdir -p "/etc/netns/$ns"
  echo "nameserver 10.10.0.115" | sudo tee "/etc/netns/$ns/resolv.conf" >/dev/null
done

# ---- LANs with bridges (same as M3) ----
attach_lan() { # <bridge> <router-ns> <router-ip/prefix> <gateway-ip> "<host:ip/prefix> ..."
  local br=$1 rns=$2 rip=$3 gw=$4 hosts=$5 h ns ip short
  sudo ip -n "$rns" link add "$br" type bridge
  sudo ip -n "$rns" link set "$br" up
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
attach_lan br-a ns-r1 10.10.0.1/26  10.10.0.1  "ns-ha1:10.10.0.2/26 ns-ha2:10.10.0.3/26 ns-ha3:10.10.0.4/26"
attach_lan br-b ns-r3 10.10.0.65/28 10.10.0.65 "ns-hb1:10.10.0.66/28 ns-hb2:10.10.0.67/28"
# DNS subnet: bridge br-dns inside R4, R4 side is 10.10.0.113, hosts use it as default gateway
attach_lan br-dns ns-r4 10.10.0.113/29 10.10.0.113 "ns-auth:10.10.0.114/29 ns-res:10.10.0.115/29 ns-auth-b:10.10.0.116/29"

# ---- point-to-point links ----
link() { # <nsA> <ifA> <ipA> <nsB> <ifB> <ipB>
  sudo ip link add "$2" netns "$1" type veth peer name "$5" netns "$4"
  sudo ip -n "$1" addr add "$3" dev "$2"; sudo ip -n "$1" link set "$2" up
  sudo ip -n "$4" addr add "$6" dev "$5"; sudo ip -n "$4" link set "$5" up
}
link ns-r1 ns-r1-eth1 10.10.0.81/30 ns-r2 ns-r2-eth0 10.10.0.82/30
link ns-r2 ns-r2-eth1 10.10.0.85/30 ns-r3 ns-r3-eth0 10.10.0.86/30
link ns-r1 ns-r1-eth2 10.10.0.89/30 ns-r4 ns-r4-eth0 10.10.0.90/30
link ns-r2 ns-r2-eth2 10.10.0.93/30 ns-r4 ns-r4-eth1 10.10.0.94/30
link ns-r3 ns-r3-eth2 10.10.0.97/30 ns-r4 ns-r4-eth2 10.10.0.98/30
link ns-r4 ns-r4-eth3 10.10.0.101/30 ns-edge ns-edge-eth0 10.10.0.102/30

# edge <-> Ubuntu: the Ubuntu end stays in the root namespace
sudo ip link add ns-edge-eth1 netns ns-edge type veth peer name ubuntu-priv
sudo ip -n ns-edge addr add 10.10.0.105/30 dev ns-edge-eth1
sudo ip -n ns-edge link set ns-edge-eth1 up
sudo ip addr add 10.10.0.106/30 dev ubuntu-priv
sudo ip link set ubuntu-priv up

for r in ns-r1 ns-r2 ns-r3 ns-r4 ns-edge; do
  sudo ip netns exec "$r" sysctl -qw net.ipv4.ip_forward=1
done

# ---- static routes: one line per router, "<subnet>=<next-hop>" ----
add_routes() {
  local ns=$1; shift
  local e
  for e in "$@"; do sudo ip -n "$ns" route add "${e%%=*}" via "${e##*=}"; done
}
add_routes ns-r1 "$B=10.10.0.82" "$L23=10.10.0.82" "$L24=10.10.0.82" \
                 "$L34=10.10.0.90" "$L4E=10.10.0.90" "$LEU=10.10.0.90"
add_routes ns-r2 "$A=10.10.0.81" "$B=10.10.0.86" "$L34=10.10.0.86" "$DNS=10.10.0.94" \
                 "$L14=10.10.0.94" "$L4E=10.10.0.94" "$LEU=10.10.0.94"
add_routes ns-r3 "$A=10.10.0.85" "$L12=10.10.0.85" \
                 "$L14=10.10.0.98" "$L24=10.10.0.98" "$L4E=10.10.0.98" "$LEU=10.10.0.98"
add_routes ns-r4 "$A=10.10.0.89" "$L12=10.10.0.89" "$B=10.10.0.97" \
                 "$L23=10.10.0.93" "$LEU=10.10.0.102"
add_routes ns-edge "$A=10.10.0.101" "$B=10.10.0.101" "$L12=10.10.0.101" "$L23=10.10.0.101" \
                   "$L14=10.10.0.101" "$L24=10.10.0.101" "$L34=10.10.0.101" "$DNS=10.10.0.101"

# Ubuntu root namespace: everything inside the lab is behind edge
for s in $A $B $L12 $L23 $L14 $L24 $L34 $L4E $DNS; do
  sudo ip route add "$s" via 10.10.0.105
done

# ---- default routes: everything unknown goes toward the internet ----
# R2 gets none: no host traffic to the internet goes through R2.
sudo ip -n ns-r1   route add default via 10.10.0.90    # R4 eth0
sudo ip -n ns-r3   route add default via 10.10.0.98    # R4 eth2
sudo ip -n ns-r4   route add default via 10.10.0.102   # edge eth0
sudo ip -n ns-edge route add default via 10.10.0.106   # ubuntu-priv

# ---- Ubuntu root namespace: router + NAT ----
# The root namespace is NOT deleted by teardown, so every rule here carries the
# comment "dns-lab". teardown.sh finds and deletes the rules by that comment.
UPLINK=$(ip route show default | awk '{print $5; exit}')   # usually eth0
sudo sysctl -qw net.ipv4.ip_forward=1
# Docker sets the FORWARD policy to DROP, so allow lab -> internet, and only replies back.
sudo iptables -I FORWARD -i ubuntu-priv -o "$UPLINK" -m comment --comment dns-lab -j ACCEPT
sudo iptables -I FORWARD -i "$UPLINK" -o ubuntu-priv -m conntrack --ctstate ESTABLISHED,RELATED \
  -m comment --comment dns-lab -j ACCEPT
# # DNS DROP rules (optional experiment). -I puts a rule at the top, so these must come AFTER
# # the ACCEPTs above, otherwise the ACCEPTs end up first and the DROPs never match.
# sudo iptables -I FORWARD 1 -i ubuntu-priv -p udp --dport 53 -m comment --comment dns-lab -j DROP
# sudo iptables -I FORWARD 1 -o ubuntu-priv -p udp --sport 53 -m comment --comment dns-lab -j DROP
# sudo iptables -I INPUT 1 -i ubuntu-priv -p udp --dport 53 -m comment --comment dns-lab -j DROP
# The source of a lab packet is the host (10.10.0.2, 10.10.0.66, ...), not 10.10.0.106.
# MASQUERADE uses the current address of the uplink (172.x, it can change after a WSL restart).
sudo iptables -t nat -A POSTROUTING -s 10.10.0.0/24 -o "$UPLINK" -m comment --comment dns-lab -j MASQUERADE

# ---- checks ----
for target in 10.10.0.66 10.10.0.102 10.10.0.106 10.10.0.114 10.10.0.115 10.10.0.116; do
  sudo ip netns exec ns-ha1 ping -c 1 -W 2 "$target" >/dev/null \
    && echo "ha1 -> $target OK" || echo "ha1 -> $target FAILED"
done
ping -c 1 -W 2 10.10.0.2 >/dev/null && echo "ubuntu -> ha1 OK" || echo "ubuntu -> ha1 FAILED"
for h in ns-ha1 ns-ha2 ns-hb1 ns-hb2 ns-auth ns-res ns-auth-b; do
  sudo ip netns exec "$h" ping -c 1 -W 2 8.8.8.8 >/dev/null \
    && echo "$h -> 8.8.8.8 OK" || echo "$h -> 8.8.8.8 FAILED"
done
sudo ip netns exec ns-ha1 traceroute -n 10.10.0.106

# ---- DNS checks (they FAIL until you start NSD and Unbound by hand in their namespaces) ----
echo "--- DNS ---"
sudo ip netns exec ns-ha1 dig +short +time=2 +tries=1 @10.10.0.115 example.com | grep -q . \
  && echo "ha1 -> dns 10.10.0.115 (ns-res) OK" || echo "ha1 -> dns 10.10.0.115 (ns-res) FAILED"
sudo ip netns exec ns-ha1 dig +short +time=2 +tries=1 @8.8.8.8 example.com | grep -q . \
  && echo "ha1 -> dns 8.8.8.8 OK" || echo "ha1 -> dns 8.8.8.8 FAILED"
