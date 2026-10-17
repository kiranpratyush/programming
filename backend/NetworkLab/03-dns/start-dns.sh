#!/usr/bin/env bash
# Starts NSD (ns-auth, ns-auth-b) and Unbound (ns-res) in their namespaces, in the background.
# Run ./build.sh first. Logs go to ./logs/<ns>.log.   ./start-dns.sh stop   stops them again.
set -euo pipefail
cd "$(dirname "$0")"
DIR=$PWD

stop() {
  sudo pkill -f "nsd -d -c $DIR/" 2>/dev/null || true
  sudo pkill -f "unbound -d -c $DIR/" 2>/dev/null || true
}

if [ "${1:-}" = stop ]; then stop; echo "stopped"; exit 0; fi

stop            # no old copies still holding port 53
mkdir -p logs

# config checks first, so a typo shows up here and not as a silent crash
nsd-checkzone lab.internal ns-auth/lab.internal.zone
nsd-checkzone b.lab.internal ns-auth-b/b.lab.internal.zone
unbound-checkconf ns-res/unbound.conf

sudo ip netns exec ns-auth   nsd -d -c "$DIR/ns-auth/nsd.conf"     >logs/ns-auth.log   2>&1 &
sudo ip netns exec ns-auth-b nsd -d -c "$DIR/ns-auth-b/nsd.conf"   >logs/ns-auth-b.log 2>&1 &
sudo ip netns exec ns-res    unbound -d -c "$DIR/ns-res/unbound.conf" >logs/ns-res.log 2>&1 &
sleep 2

# checks from ns-ha1
q() { # <label> <server> <name>
  sudo ip netns exec ns-ha1 dig +short +time=2 +tries=1 +norecurse "@$2" "$3" | grep -q . \
    && echo "$1 OK" || echo "$1 FAILED (see logs/)"
}
q "ns-auth   lab.internal      " 10.10.0.114 hb1.lab.internal
q "ns-auth-b b.lab.internal    " 10.10.0.116 hb1.b.lab.internal
sudo ip netns exec ns-ha1 dig +short +time=3 +tries=1 @10.10.0.115 hb1.b.lab.internal | grep -q . \
  && echo "ns-res     via delegation   OK" || echo "ns-res     via delegation   FAILED (see logs/)"
