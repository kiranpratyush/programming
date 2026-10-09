#!/usr/bin/env bash
# M2 teardown: deleting a namespace also removes the veth ends inside it (and their peers).
for ns in ns-ha1 ns-hb1 ns-r1 ns-r2 ns-r3; do
  sudo ip netns del "$ns" 2>/dev/null
done
exit 0
