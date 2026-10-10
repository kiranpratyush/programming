#!/usr/bin/env bash
# M5/M6 teardown. Deleting ns-edge also removes ubuntu-priv (its veth peer) and the routes via it.
for ns in ns-ha1 ns-ha2 ns-ha3 ns-hb1 ns-hb2 ns-r1 ns-r2 ns-r3 ns-r4 ns-edge; do
  sudo ip netns del "$ns" 2>/dev/null
done
sudo ip link del ubuntu-priv 2>/dev/null

# Root namespace rules stay after the namespaces are gone. Delete each rule with the
# comment "m5-lab": print it with -S, change -A to -D, run it again.
for t in filter nat; do
  sudo iptables -t "$t" -S | grep -- '--comment m5-lab' | sed 's/^-A /-D /' |
    while read -r rule; do sudo iptables -t "$t" $rule; done
done
# net.ipv4.ip_forward stays 1 in the root namespace (Docker and WSL also use it).
exit 0
