# Next steps (resume file)

Last update: 2026-10-08

## Status
| Milestone | State |
|---|---|
| M0 design | Done. Routing tables checked. Two small tasks remain (see "Open items"). |
| M1 one router | Done. Proof captured. Log written. |
| M2 chain of three routers | Not started. |
| M3 to M6 | Not started. |

## Open items from M1 and M0
1. Run `sudo ./build.sh` once. It was not run yet. Paste any error to Claude.
2. Fix the `via x.x.x.x/NN` notation in the M0 route tables. A gateway has no prefix. Example: `via 10.10.0.82`.
3. Pick one host name style. The README uses `ns-lanahost1` and `ns-ha1`. Use `ns-ha1`.
4. Answer the M0 questions in writing:
   - Why can LAN-A not be a /27? (A /27 has 30 usable addresses. LAN-A needs 51.)
   - Why is a /30 enough for a link? Why does a /31 also work?
   - Write the network address and broadcast address of each subnet.

## M2: chain of three routers (start here)
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
