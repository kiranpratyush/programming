# Project: Add DNS to the namespace network

Goal: hosts in the lab use names instead of IPs. `ns-ha1` runs `curl http://web.lab.internal:8000` and it works. You run your own **authoritative server**, your own **recursive resolver** and a **delegated sub-zone**. For every step you can show the packet that made it happen.

This builds on `../02-virtual-network/m5-build.sh` (the M5 network with internet access through NAT). Write the scripts and config files **yourself**. Claude reviews them; Claude does not write them.

---

## Definition of done (this phase is not finished until all of these are true)

1. **The network:** `03-dns/build.sh` brings up M5 + DNS with one command, and `teardown.sh` removes it. After a fresh build:
   - `ip netns exec ns-ha1 getent hosts hb1.lab.internal` → `10.10.0.66`
   - `ip netns exec ns-ha1 getent hosts example.com` → a real internet IP
   - `ip netns exec ns-ha1 traceroute 10.10.0.66` shows **router names**, not just IPs (reverse DNS)
2. **The log:** a "DNS" session entry in `network.md` for each milestone, written in your own words: prediction, proof, break it.
3. **The learning artifact:** one page, _"How a name becomes an address in my network"_. It follows one query from `ns-ha1` to the answer, step by step, using **your own captures** from this lab. You write the text. Claude reviews it and helps publish it as a page you can share.

When you can't explain a step without notes, the step is not done.

---

## Target design

```text
   LAN-A                                                      LAN-B
 ha1 ha2 ha3 ── br-a ── R1 ── R2 ── R3 ── br-b ── hb1 hb2
                         \    |    /
                          `── R4 ─'
                              |
                            edge ── Ubuntu ── internet (root servers, .com, ...)

 New in this phase (you choose where they attach, see D0):
   ns-auth   authoritative only : the zones lab.internal and 0.10.10.in-addr.arpa
   ns-res    recursive only     : every host asks it. It asks ns-auth for lab names
                                  and the real internet for everything else.
   ns-auth-b authoritative only : the delegated sub-zone b.lab.internal (D5)
```

Why `.internal`: ICANN reserved this top-level domain for private networks (2024). It will never exist on the public internet, so your names can't collide with real ones.

Why two separate servers: one server that does both jobs (like dnsmasq or the resolver in your home router) hides the boundary between them. That boundary is the most important idea in DNS. Keep the jobs separate.

Suggested software (one tool for each job):

- **NSD**: authoritative only. It can't recurse, so you can't mix up the roles by accident.
- **Unbound**: recursive only.
- `dig` (package `bind9-dnsutils`), `tcpdump`, `getent`.

Install: `sudo apt install nsd unbound bind9-dnsutils`. Then **stop the services that apt starts** (`systemctl disable --now nsd unbound`). Run both programs in the foreground inside the namespaces, with your own config files from this folder.

---

## D0: Design on paper

Write your answers under "My design" at the end of this file.

1. Where do `ns-auth`, `ns-res` and `ns-auth-b` attach? Do they go on a LAN bridge or on a new subnet? Pick addresses. Which routes must change?
2. Write the zone `lab.internal` on paper. Give each host and each router a name. Include at least: SOA, NS, A, CNAME (`web` → `hb1`), TXT, MX and one SRV.
3. Draw the path of a query for `hb1.lab.internal` and of a query for `example.com`. Mark which hops are recursive (RD=1) and which are iterative (RD=0).

Questions:

1. I'll put `ns-auth`,ns-res , ns-auth-b attached to R4 , I am planning to put a bridge and to the bridge these ns-res,ns-auth,ns-auth-b will be connected via eth-pair, one more pair will be connected to the R4 and the bridge. The bride is a Link layer device so it does not needs an IP address in its interface.
2. This will create a new subnet , in this new subnet we need 3 devices + 1 gateway + 2(network and broad cast address) = 6 addresses. round it to 8 addresses so host bit 3 and network bit is 29 bits
3. As I can not reuse the 10.10.0.108 so I can use the 10.10.0.112/29-10.10.0.119/29 with a bridge naming

bridge: br-dns
network address: 10.10.0.112/29
broadcast address : 10.10.0.119/29
R4 ns-r4-eth4 : 10.10.0.113/29
ns-auth : 10.10.0.114/29 (ideally the interface ns-auth-eth0)
ns-res: 10.10.0.115/29 (ideally the interface ns-res-eth0)
ns-auth-b: 10.10.0.116/29 (ideally the interface ns-auth-b-eth0)
so in each host,edge,routers except R4,ubuntu rootnamespace there will be single route should be added to ns-res, as the other servers are in the bridge and we won't directly connect with them we don't need them in the route table.

- Why does an NS record that points to a name **inside** its own zone need a glue A record?
- What does each SOA field do (serial, refresh, retry, expire, minimum)? Which one controls negative caching?
- `/etc/hosts`, the stub resolver in glibc, the recursive resolver, the authoritative server: who reads which file, and who sends packets?

#### My answer

1. I am planning to attach ns-auth

### My Design:

.internal: Use a top level domain inside a Lan or Private networks. so for example I can assign a domain name to IP address. When I should query let's say ha1.lab.internal or ha2.lab.internal it should give the IP 10.10.0.2 and 10.10.0.3/26 respectively.
similarly if I say hb1.lab.internal or hb2.lab.internal it should give IP 10.10.0.66/28 or 10.10.0.67/28 respectively.

## D1: Observe DNS in the lab you already have (Done)

Nothing new to install. Bring up `m5-build.sh`.

- **Predict first:** does `ip netns exec ns-ha1 getent hosts example.com` work right now? Which server does it ask, and why that one? (Look at what `ip netns exec` does with `/etc/netns/<name>/resolv.conf`. This is your first "read only what the task forces you to read".)
- **Proof:** `dig @8.8.8.8 example.com` from `ns-ha1`. Run tcpdump on both sides of `ubuntu-priv` and the uplink. Show: the source port, the query ID, the answer, and where NAT changes the source.
- **Break it:** drop UDP 53 at edge. What does `dig` do (timeout, retry, how many tries)? Then allow UDP and drop TCP 53. Find a query whose answer is too big for UDP: the server sets the TC flag and the client must retry over TCP. (Hint: `dig +bufsize=512 TXT <a domain with many TXT records>`. Compare with and without `+ignore`.)

### What I did and what I learnt

In My existing set up I did dig to recursive resolver 8.8.8.8 from my ns-ha1 for example.com. It did a query via ubuntu-priv as I have NAT set up it went via eth0 in root namespace and I got two ip addresses A record.
Then in iptables in FORWARD chain I put the DROP rule on UDP 53 and when I did dig again packet dropped at ubuntu-priv. I saw it via tcpdump -ni ubuntu-priv -vv. Then I forced dig to use tcp packet now it passed through because I only droped the UDP packets.

## D2: Authoritative server

`ns-auth` runs NSD with the zone `lab.internal`.

- **Proof:** `dig @<ns-auth> hb1.lab.internal` returns the answer with the `aa` flag. Explain each flag in the header (`qr aa rd ra`).
- **Break it:**
  - Ask `ns-auth` for `example.com`. What comes back, and why is `ra` missing?
  - Change an A record **without** changing the serial. Does anything break today? Write when it would break (secondary servers).
  - Ask for a name that doesn't exist. Find the SOA in the AUTHORITY section and explain why it is there.

## D3: Recursive resolver

`ns-res` runs Unbound. Lab names go to `ns-auth` (stub zone). Everything else uses **full recursion from the root servers**, through your NAT. No forwarding to 8.8.8.8.

- Point each host to `ns-res` with `/etc/netns/<ns>/resolv.conf`.
- **Proof:**
  - `getent hosts hb1.lab.internal` and `getent hosts example.com` work from `ns-ha1`.
  - tcpdump on `ns-res` during **one** cold query for `example.com`: list each server it asked (root → `.com` → `example.com` servers). Compare with `dig +trace`.
  - Ask the same name twice. Show the query time and the TTL counting down. Show that the second query sends no packets upstream.
- **Break it:**
  - Kill NSD. Which lab names still resolve, and for how long? Why?
  - Query a lab name that does not exist, then add it to the zone. Why does it still fail for a while? (negative caching, SOA minimum)

## D4: Reverse DNS

Add the zone `0.10.10.in-addr.arpa` to `ns-auth` with PTR records for every interface.

- **Proof:** `traceroute` from `ha1` to `hb1` shows router names. `dig -x 10.10.0.82`.
- **Question:** why is the reverse name written backwards? Who owns the reverse zone for a real public IP?

## D5: Delegation (how the real hierarchy works)

`ns-auth-b` is authoritative for `b.lab.internal`. In `lab.internal`, add the NS record for the sub-zone and its glue record.

- **Proof:** walk it yourself with `dig +norecurse`. Ask `ns-auth` for `hb1.b.lab.internal`. You get a **referral** (no answer, NS in AUTHORITY, glue in ADDITIONAL). Then ask `ns-auth-b`. Your resolver does exactly these steps with the real root.
- **Break it:**
  - Remove the glue record. What happens?
  - Make the parent NS record and the child NS record disagree (a "lame delegation"). How does the resolver show it?

## D6: Compare with the real world

| You built                       | Find it in the real world                                                          |
| ------------------------------- | ---------------------------------------------------------------------------------- |
| `/etc/netns/ns-ha1/resolv.conf` | WSL's `/etc/resolv.conf`, `systemd-resolved` on 127.0.0.53 (`resolvectl status`)   |
| `ns-res`                        | Docker's embedded DNS on `127.0.0.11` inside a container on a user-defined network |
| `ns-auth` + `lab.internal`      | A Route 53 private hosted zone, Kubernetes CoreDNS (`svc.cluster.local`)           |
| delegation to `ns-auth-b`       | `dig +trace` of a real domain: the NS records at the `.com` level                  |

- **Backend problem (from `network.md` Stage 6):** change the A record of `web` while a client keeps a pooled HTTP connection open. Does the client follow the change? Why? What does that mean for TTLs during a deployment?

---

## Rules (same as the network lab)

1. Design before you build. Predict before every proof, and write the prediction in the log first.
2. Script everything. Rebuilding from scratch takes one command.
3. One milestone per session.
4. Read only what the current step needs: man pages first (`man nsd.conf`, `man unbound.conf`, `man resolv.conf`), then RFC sections when a capture surprises you. RFC 1034 section 4.3.2 (the resolution algorithm) is worth reading at D5.

---

## My design

(D0 goes here)
