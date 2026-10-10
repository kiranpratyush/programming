# Next steps (resume file)

Last update: 2026-10-09

## Status
| Milestone | State |
|---|---|
| M0 design | Done. Routing tables checked. Two small tasks remain (see "Open items"). |
| M1 one router | Done. Proof captured. Log written. |
| M2 chain of three routers | Script built (`m2-build.sh`). |
| M3 | Script built (`m3-build.sh`). |
| M5 first pass | Built (`m5-build.sh`): R4, edge and Ubuntu link. Static return routes, no NAT. |
| M5 second pass (iptables and NAT) | Mostly done: INPUT/FORWARD filter, conntrack, SNAT, MASQUERADE, hosts ping 8.8.8.8 (`m5-build.sh`). N6 DNAT not done. |
| M6 redundancy | Postponed (does not block Phase B). |

## Open items from M1 and M0
1. Run `sudo ./build.sh` once. It was not run yet. Paste any error to Claude.
2. Fix the `via x.x.x.x/NN` notation in the M0 route tables. A gateway has no prefix. Example: `via 10.10.0.82`.
3. Pick one host name style. The README uses `ns-lanahost1` and `ns-ha1`. Use `ns-ha1`.
4. Answer the M0 questions in writing:
   - Why can LAN-A not be a /27? (A /27 has 30 usable addresses. LAN-A needs 51.)
   - Why is a /30 enough for a link? Why does a /31 also work?
   - Write the network address and broadcast address of each subnet.

## Close Phase B: next three steps (start here)
Phase B in network.md: "Layer 3 in Linux → build containers by hand → compare with real Docker". Do these steps in this order.

### Step 1: write the log (one session)
The log in `network.md` stops at Session 3. Write a Session 4 entry from memory, in your own words, for M1 to M5 and NAT. Then Claude checks it. Include:
- The netfilter hook diagram (PREROUTING, INPUT, FORWARD, OUTPUT, POSTROUTING).
- How the routing decision chooses INPUT or FORWARD (local table, weak host model).
- The conntrack states (NEW, ESTABLISHED, RELATED, INVALID). Why one ESTABLISHED,RELATED rule is enough for replies.
- Why SNAT is in POSTROUTING and DNAT is in PREROUTING.
- DROP, REJECT and "Net Unreachable": who sends what.
- The NATs from ha1 to 8.8.8.8, and which device does each one.

### Step 2: DNAT / port forward (step N6, one session)
Why: `docker run -p`, a Kubernetes Service and an AWS load balancer all use this idea.
1. Run `python3 -m http.server 8000` on `ns-hb1`.
2. On edge, DNAT `10.10.0.105:8080` to `10.10.0.66:8000`.
3. Predict first: which source IP does hb1 see? Why does the reply need no rule?
4. Proof: `curl 10.10.0.105:8080` from Ubuntu works. tcpdump on both sides of edge shows the destination change.
5. Break it: put the DNAT rule in POSTROUTING. Explain the result.
6. Add the rule to `m5-build.sh` (it is in `ns-edge`, so teardown removes it).

### Step 3: real Docker (Stage 24, one or two sessions). This is the end of Phase B.
Find each part that you built by hand in Docker:

| You built | Find it in Docker |
|---|---|
| `ip netns add` | `docker inspect -f '{{.State.Pid}}'` + `nsenter -t <pid> -n` |
| veth pair | `ip link` on the host (`vethXXXX@ifN`) |
| bridge `br-a` | `docker0` and `br-<id>` (`bridge link`) |
| host default route via 10.10.0.1 | `ip route` in the container (via 172.17.0.1) |
| MASQUERADE on Ubuntu | `iptables -t nat -S POSTROUTING` |
| DNAT (step 2) | `-p 8080:80` → the `DOCKER` chain |
| FORWARD DROP + ESTABLISHED rule | the `DOCKER-USER` and `DOCKER-FORWARD` chains |

Break it: delete Docker's MASQUERADE rule. Predict which traffic stops, then diagnose it with `ip`, `ping`, `tcpdump` and `conntrack`. Restart Docker to restore the rule.

### Postponed
- M6 redundancy (metric, longest prefix match): useful for AWS route tables. Do it later for more route practice.
- N9 nftables: Docker and Kubernetes still show iptables syntax. Learn nft when you need it.
- tc/netem: fits Phase C (timeouts and retries under packet loss).

After step 3: start Phase C (HTTP, TLS, nginx proxy and load balancer in docker compose, with `tc netem`).

## M5 second pass: iptables and NAT (reference)

### The problem
In the M5 first pass, the Ubuntu root namespace has static routes back to each lab subnet. A real ISP does not have routes to your private subnets. NAT solves this problem: "my upstream cannot route back to my private addresses."

### Mental model (learn this before you write rules)
Each namespace has its own netfilter rules. A rule in `ns-edge` has no effect on `ns-r4`.

```
            in-iface                                      out-iface
 packet ──► PREROUTING ──► [routing decision] ──► FORWARD ──► POSTROUTING ──►
            (DNAT here)          │                (filter)    (SNAT here)
                                 ▼                                ▲
                               INPUT ──► local process ──► OUTPUT ┘
                             (to this box)              (from this box)
```

- Tables say what a rule does: `filter` accepts or drops, `nat` rewrites addresses.
- Chains say where in the path the rule runs.
- DNAT is in PREROUTING. The destination must change before the routing decision.
- SNAT is in POSTROUTING. The kernel must know the output interface first.
- conntrack is the base of NAT. The `nat` table sees only the first packet of a connection. Conntrack applies the same translation to all other packets, and the reverse translation to the replies.

### Steps (one step per session: predict, build, prove, break)
| Step | Goal | What you do | Proof |
|---|---|---|---|
| N1 Observe | Read rules and counters | Run `iptables -L -v -n` and `iptables -t nat -L -v -n` in `ns-edge`. Add a `LOG` rule in FORWARD. Run a ping. | The counters go up. Log lines appear in `dmesg`. |
| N2 Filter | Stateless, then stateful | On edge, set the FORWARD policy to DROP. Allow only lab to Ubuntu. Then add `-m conntrack --ctstate ESTABLISHED,RELATED -j ACCEPT`. | Without the conntrack rule, replies are dropped. With it, they pass. |
| N3 Conntrack | See the state table | Install `conntrack`. Run `conntrack -L` in `ns-edge` during a ping. | You see one entry with both directions. |
| N4 SNAT | The main fix | Break: remove the Ubuntu return routes. The ping from ha1 fails. Fix: on edge, add `-t nat -A POSTROUTING -o ns-edge-eth1 -j SNAT --to 10.10.0.105`. | tcpdump on both sides of edge. Inside, the source is 10.10.0.2. Outside, the source is 10.10.0.105. |
| N5 MASQUERADE | Compare with SNAT | Replace SNAT with MASQUERADE. | Explain when each is correct (fixed IP or dynamic IP). |
| N6 DNAT | Port forward | Run `python3 -m http.server` on hb1. On edge, DNAT `10.10.0.105:8080` to `10.10.0.66:8000`. | `curl 10.10.0.105:8080` from Ubuntu works. Predict first: which source IP does hb1 see? |
| N7 Real internet | Two NAT layers | Make ha1 reach `1.1.1.1`. Add a default route on edge to Ubuntu. Add MASQUERADE in the root namespace on `eth0`. | `ping 1.1.1.1` from ha1 works. Explain where each address change occurs. Count the NATs. |
| N8 Docker | Connect to real systems | Read `iptables -t nat -S` on a host that runs Docker. | Map each Docker rule to a step above (DOCKER chain = N6, MASQUERADE = N5). |
| N9 nftables | Modern syntax | Write the N4 and N6 rules again in `nft`. | The same proofs pass. |

### Questions to answer in the log
- Why must DNAT occur before the routing decision?
- In N4, how does edge know which inside host must get the reply?
- In N6, why does the reply not need its own rule?
- What occurs if two inside hosts use the same source port at the same time? How does NAT fix it?

### Notes for this environment
- WSL2: run `iptables --version`. If it shows `nf_tables`, the `iptables` commands write nftables rules. This is correct. Do not mix it with `iptables-legacy`.
- N7 changes the rules in the root namespace. Remove those rules in the teardown script. If you do not, they stay after teardown.
- Do N4 before N7. N4 has no real internet traffic, so nothing from outside can confuse you.
- Script names: `m5-nat-build.sh` and `m5-nat-teardown.sh`. You write them. Claude reviews them.

## M2: chain of three routers
Topology: `ns-ha1 ── ns-r1 ── ns-r2 ── ns-r3 ── ns-hb1`

Use the addresses from the M0 design:

| Namespace | Interface | Address |
|---|---|---|
| ns-ha1 | ns-ha1-eth0 | 10.10.0.2/26 (default via 10.10.0.1) |
| ns-r1 | ns-r1-eth0 | 10.10.0.1/26 |
| ns-r1 | ns-r1-eth2 | 10.10.0.81/30 |
| ns-r2 | ns-r2-eth0 | 10.10.0.82/30 |
| ns-r2 | ns-r2-eth1 | 10.10.0.85/30 |
| ns-r3 | ns-r3-eth0 | 10.10.0.86/30 |
| ns-r3 | ns-r3-eth1 | 10.10.0.65/28 |
| ns-hb1 | ns-hb1-eth0 | 10.10.0.66/28 (default via 10.10.0.65) |

Static routes (from your M0 design, with R4 left out):
- R1: `10.10.0.64/28 via 10.10.0.82` and `10.10.0.84/30 via 10.10.0.82`
- R2: `10.10.0.0/26 via 10.10.0.81` and `10.10.0.64/28 via 10.10.0.86`
- R3: `10.10.0.0/26 via 10.10.0.85` and `10.10.0.80/30 via 10.10.0.85`

Remember: set `net.ipv4.ip_forward=1` on R1, R2 and R3. A host does not need it.

### Steps
1. Write your prediction in the log first. Answer: what does `traceroute -n` show? Which IP does each hop show?
2. Write `build-m2.sh` and `teardown-m2.sh` yourself. Claude reviews them.
3. Run `sudo apt install traceroute` if it is not installed.
4. Proof: run `traceroute -n 10.10.0.66` from `ns-ha1`. It must list three hops. Explain how traceroute uses TTL.
5. Break it: remove only the return route on R2 (`10.10.0.0/26 via 10.10.0.81`). The ping goes out, but the reply is lost. Find where it stops with tcpdump on each link.
6. Write the M2 log entry.

### Questions to answer in the log
- Which interface IP does each traceroute hop show? Is it the incoming interface or the outgoing interface?
- Why does every router need a route back?
- With the return route removed on R2, which hop sees the request but not the reply?

## Rules (from the README)
1. Design before you build.
2. Script everything. Rebuild with one command.
3. Predict before every proof. Write the prediction in the log first.
4. One milestone per session. Do not start the next milestone before you can explain the current one without notes.

## Working agreement with Claude
- You build by hand first. Claude verifies your commands and output.
- For M2, you write the scripts. Claude reviews them. (Claude wrote the M1 scripts.)
- Claude writes in ASD-STE100 (Simplified Technical English).
- Claude cannot run `sudo` here. You run the commands and paste the output.
