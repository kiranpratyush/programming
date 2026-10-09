#!/usr/bin/env bash
# M5/M6 teardown. Deleting ns-edge also removes ubuntu-priv (its veth peer) and the routes via it.
for ns in ns-ha1 ns-ha2 ns-ha3 ns-hb1 ns-hb2 ns-r1 ns-r2 ns-r3 ns-r4 ns-edge; do
  sudo ip netns del "$ns" 2>/dev/null
done
sudo ip link del ubuntu-priv 2>/dev/null
exit 0
