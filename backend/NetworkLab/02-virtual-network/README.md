# Project: Build a multi-router network inside one Linux machine

Goal: build a small "internet" out of network namespaces (hosts and routers), veth cables and Linux bridges (switches). Watch every hop, design every subnet yourself, then break it on purpose and diagnose it.

Everything runs in WSL Ubuntu. Write the build scripts **yourself**; ask Claude to review them, not to write them.

---

## Target topology (final state)

```text
        LAN-A (switch: br-a)
  hA1 ─┐
  hA2 ─┼── br-a ─── R1 ─────── R2 ─────── R3 ─── br-b ─┬─ hB1
  hA3 ─┘             \          |          /            └─ hB2
                      \         |         /           LAN-B (switch: br-b)
                       └─────── R4 ──────┘      ← M6: backup path
                                |
                              edge ──── Ubuntu (root namespace) ──── real internet   ← M5
```

- **Hosts** (`hA1`, `hB1`, …): namespaces with one interface and a default route.
- **Routers** (`R1`–`R4`): namespaces with several interfaces and `net.ipv4.ip_forward=1`.
- **Switches** (`br-a`, `br-b`): Linux bridges. They connect many hosts on one street; they don't route.
- **Links between routers**: veth pairs, each its own tiny subnet.

---

## M0: Design on paper (no computer)

You get the address block **`10.10.0.0/16`**. Split it yourself:

| Network | Requirement |
|---|---|
| LAN-A | must fit **50 hosts** |
| LAN-B | must fit **10 hosts** |
| Each router↔router link | as small as possible (only 2 addresses needed) |

Deliverables, in this README under "My design":
1. Topology drawing with every interface name.
2. An address table: subnet, prefix, usable range, gateway, and which interface gets which IP.
3. For each router: its full routing table **on paper** (which subnets are connected, which need `via`).

Questions to answer: why can't LAN-A be a `/27`? Why is a `/30` (or `/31`) enough for a link? What's the network address and broadcast address of each subnet?

---

## Milestones

Each milestone ends with **proof** (an observable result) and a **log entry** in your own words.

### M1: One router between two hosts
`hA1 ── R1 ── hB1`, using two different subnets.
- Learn: what makes a namespace a router (`ip_forward`), and why hosts need a `default via`.
- **Proof:** `hA1` pings `hB1`. Capture with `tcpdump -n -e` on **both** sides of R1:
  - The MACs are different on each side, but the IPs are the same → a router rewrites the frame, not the packet.
  - The **TTL** in the packet is 1 lower on the far side (`tcpdump -v` shows `ttl`).
- **Break it:** turn `ip_forward` off. What does the ping do: timeout, refused or unreachable? Why?

### M2: A chain of three routers (static routing)
`hA1 ── R1 ── R2 ── R3 ── hB1`
- Learn: static routes (`ip route add <subnet> via <next-hop>`), and that **every router needs a route back**.
- **Proof:** `traceroute -n` from `hA1` to `hB1` lists each router hop. Explain how traceroute uses TTL to find them.
- **Break it:** remove *only* the return route on R2. Ping now goes out, but the reply gets lost. Find where it's lost using tcpdump at each hop. (This is the most common real-world routing bug.)

### M3: A real LAN with a switch
Replace single hosts with `br-a` (3 hosts) and `br-b` (2 hosts).
- Learn: a bridge is a switch. One broadcast domain. ARP is shared across the LAN.
- **Proof:** ARP from `hA1` for `hA2` reaches `hA3` too (tcpdump on `hA3`). `bridge fdb show` shows the switch's MAC table after the pings.
- **Question:** why does `hA1` → `hA2` never touch R1?

### M4: Fault-injection drills
Ask Claude for a "chaos script" that secretly breaks one thing; then diagnose it using only `ip`, `ping`, `traceroute` and `tcpdump`. Possible faults:
- a wrong prefix length on one interface (`/24` vs `/26`)
- overlapping subnets
- `ip_forward` off on one router
- a missing return route
- a routing loop (R1 → R2 → R1 …). Watch TTL run out, and the `Time to live exceeded` message.
- a wrong gateway on a host (an IP not on its own subnet)

Write each diagnosis as: symptom → evidence → cause → fix.

### M5: Connect to the real internet (NAT)
Add `edge` and connect it to Ubuntu's root namespace.
- Learn: a default route chain (host → R1 → … → edge → Ubuntu → Windows → home router), plus **MASQUERADE** (NAT) on the way out.
- **Proof:** `hA1` pings `8.8.8.8`. Capture before and after the NAT point and show the source IP change. Then count how many NATs a packet from `hA1` goes through.
- AWS mapping: LAN = subnet, router tables = VPC route tables, edge + MASQUERADE = NAT Gateway, Ubuntu's uplink = Internet Gateway.

### M6: Redundancy and route preference
Add R4 as a second path between R1 and R3.
- Learn: two routes to the same subnet, choosing between them with **longest prefix match** and **metric**, and failover by removing a link.
- **Proof:** traceroute before and after `ip link set <link> down`. Make one specific `/32` take a different path from the rest of its `/24`.

### Stretch goals (later)
- Replace static routes with dynamic routing: run **FRR** (OSPF) in the router namespaces, or move to **containerlab**.
- Rebuild the same topology with Docker networks and compare.
- Add a firewall rule on R2 (`nft`) that drops one subnet. Compare a silent drop with a reject, and connect this back to your timeout vs refused table.

---

## Rules for this project

1. **Design before you build.** Write the routing table on paper first, then compare it with `ip route` in each namespace.
2. **Script everything**: `build.sh` (idempotent) and `teardown.sh`. Rebuilding from scratch should take one command.
3. **Predict before every proof.** Write the prediction in the log *first*.
4. **One milestone per session.** Don't start the next one until you can explain the current one without notes.

### Building blocks (look these up yourself; man pages first)
`ip netns add/exec` · `ip link add … type veth peer name …` · `ip link set … netns …` · `ip link add … type bridge` · `ip link set … master …` · `ip addr add` · `ip route add … via …` · `sysctl -w net.ipv4.ip_forward=1` (inside a router namespace) · `traceroute -n` (`sudo apt install traceroute`) · `tcpdump -n -e -v` · `bridge fdb show` · `iptables -t nat … MASQUERADE` / `nft`

---

## My design
_(fill in at M0)_

## Log
_(one entry per milestone: prediction → what happened → explanation in my own words)_
