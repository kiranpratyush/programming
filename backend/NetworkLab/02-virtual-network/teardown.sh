#!/usr/bin/env bash
# M1 teardown: delete the three namespaces. Deleting a namespace also deletes its veth ends.
set -u

for ns in ns-ha1 ns-hb1 ns-r1; do
    ip netns del "$ns" 2>/dev/null || true
done
