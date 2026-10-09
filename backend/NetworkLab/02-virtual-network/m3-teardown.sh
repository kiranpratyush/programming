#!/usr/bin/env bash
# M3 teardown: deleting namespaces removes their veth ends and bridges.
for ns in ns-ha1 ns-ha2 ns-ha3 ns-hb1 ns-hb2 ns-r1 ns-r2 ns-r3; do
  sudo ip netns del "$ns" 2>/dev/null
done
exit 0
