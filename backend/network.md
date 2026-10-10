# Backend Networking — End-to-End Hands-On Syllabus

> Goal: learn networking deeply enough to design, debug, and operate backend systems confidently.
>
> Focus: **TCP/IP, DNS, HTTP, TLS, streaming protocols, Linux networking, load balancing, containers, Kubernetes, and production failure modes.**
>
> Approach: every topic should end with an observable experiment. Do not only read — use `curl`, `ss`, `ip`, `dig`, `tcpdump`, Wireshark, `nc`, `openssl`, Docker, and small C#/Python programs.

---

## 0. How to use this syllabus

For each module:

1. Learn the mental model.
2. Run the Linux commands.
3. Build the small lab.
4. Capture the traffic.
5. Explain what happened without looking at notes.
6. Break something deliberately and diagnose it.

Recommended language for programming labs: **C# / .NET**.

Recommended environment:

- Linux or WSL2 Ubuntu
- .NET SDK
- Docker
- Wireshark
- `tcpdump`
- `iproute2`
- `dnsutils`
- `netcat-openbsd`
- `curl`
- `openssl`
- `traceroute`
- `iperf3`
- `nginx`

Ubuntu setup:

```bash
sudo apt update

sudo apt install -y \
    iproute2 \
    iputils-ping \
    dnsutils \
    netcat-openbsd \
    curl \
    tcpdump \
    traceroute \
    openssl \
    iperf3 \
    nginx
```

Useful tools you should gradually become comfortable with:

```text
ip
ss
ping
traceroute
tracepath
dig
nslookup
curl
nc
tcpdump
openssl
iperf3
ethtool
tc
nft
systemd-resolve / resolvectl
```

---

# Stage 1 — Networking mental model

## Topics

Learn:

- What a network is
- Host
- Interface
- MAC address
- IP address
- Port
- Socket
- Packet
- Frame
- Client vs server
- LAN vs WAN
- Router vs switch
- Loopback
- Network interface
- OSI model vs TCP/IP model
- Encapsulation

Do not memorize all seven OSI layers as trivia.

For backend engineering, this model is more useful:

```text
Application
    HTTP / gRPC / WebSocket / DNS
            ↓
Transport
       TCP / UDP / QUIC
            ↓
Network
            IP
            ↓
Link
   Ethernet / Wi-Fi
```

Understand encapsulation:

```text
HTTP request
    ↓
TCP segment
    ↓
IP packet
    ↓
Ethernet frame
```

## Linux practice

Run:

```bash
ip addr
ip link
ip route
ip neigh
hostname
hostname -I
```

Questions you should be able to answer:

- Which interfaces exist?
- Which IP belongs to each interface?
- What is `lo`?
- What is your default gateway?
- Which route is used for an external IP?
- What MAC addresses are currently in the neighbour table?

Try:

```bash
ip route get 8.8.8.8
```

Predict the interface before reading the output.

## Lab

Draw your machine's path to the internet:

```text
process
  ↓
socket
  ↓
Linux network stack
  ↓
interface
  ↓
router
  ↓
ISP
  ↓
internet
```

### Resource

Beej's Guide to Network Concepts  
https://beej.us/guide/bgnet0/

Optional textbook:

**Computer Networking: A Top-Down Approach — Kurose & Ross**

Do not read the entire book sequentially before doing labs. Use it as the conceptual reference for the relevant modules below.

---

# Stage 2 — IPv4, subnetting, routing, NAT

## Topics

Learn:

- IPv4 address
- Network prefix
- CIDR
- subnet mask
- network address
- broadcast address
- private address ranges
- public addresses
- default gateway
- routing table
- longest-prefix match
- NAT
- SNAT
- DNAT
- PAT / port translation

Know the common private ranges:

```text
10.0.0.0/8
172.16.0.0/12
192.168.0.0/16
```

You do not need competitive-exam-level subnet calculations.

You should be comfortable interpreting:

```text
10.20.30.17/24
10.20.0.0/16
172.16.8.0/21
```

## Linux practice

```bash
ip addr
ip route
ip route show table main
ip route get 1.1.1.1
```

Add a temporary route inside a network namespace later rather than altering your real machine.

## Practice questions

For:

```text
10.0.5.21/24
```

determine:

- network
- approximate usable host range
- whether `10.0.6.20` is in the same subnet

Then understand why:

```text
10.0.5.21 -> 10.0.5.50
```

can generally be delivered directly, while:

```text
10.0.5.21 -> 8.8.8.8
```

needs a router.

## Lab

Use two Linux network namespaces in Stage 12 to make routing real.

### Resources

Linux `ip` command / iproute2 manual  
https://man7.org/linux/man-pages/man8/ip.8.html

Linux routing manual  
https://man7.org/linux/man-pages/man8/ip-route.8.html

---

# Stage 3 — Sockets

This is where backend networking should become concrete.

## Topics

Understand:

```text
IP + port + transport protocol
```

and the idea of a socket.

Learn:

- listening socket
- accepted socket
- local endpoint
- remote endpoint
- bind
- listen
- accept
- connect
- read
- write
- close

A server conceptually does:

```text
socket()
bind()
listen()
accept()
read/write
close()
```

A client does:

```text
socket()
connect()
read/write
close()
```

## C# lab — TCP echo server

Build:

```text
TcpClient
      ↓
TcpListener
```

Server:

```text
Client: hello

Server: echo: hello
```

Use:

- `TcpListener`
- `TcpClient`
- `NetworkStream`

Do not use ASP.NET for this lab.

## Linux practice

Start the server, then:

```bash
ss -ltnp
```

Connect:

```bash
nc localhost 5000
```

Inspect established sockets:

```bash
ss -tnp
```

Useful variations:

```bash
ss -ltn
ss -tan
ss -s
```

## Questions

Explain:

```text
127.0.0.1:53000 -> 127.0.0.1:5000
```

Which side is server?

Why is `53000` different on another client?

What happens when two clients connect simultaneously?

### Resource

Beej's Guide to Network Programming  
https://beej.us/guide/bgnet/

Even if you write C#, reading the socket API explanation is useful.

Linux `ss` manual  
https://man7.org/linux/man-pages/man8/ss.8.html

---

# Stage 4 — TCP

This is one of the highest-value modules for backend engineers.

## Topics

Learn:

- connection-oriented transport
- reliable byte stream
- 3-way handshake
- SYN
- SYN-ACK
- ACK
- sequence numbers
- acknowledgement numbers
- retransmission
- duplicate ACK
- ordering
- receive window
- flow control
- congestion control — conceptual understanding
- FIN
- RST
- half-close
- TIME_WAIT
- keepalive
- MSS
- MTU at a high level

Most important fact:

> TCP is a byte stream. It does not preserve application message boundaries.

If you send:

```text
HELLO
WORLD
```

the receiver might read:

```text
HELLOWORLD
```

or:

```text
HEL
LOWOR
LD
```

## Lab 1 — inspect the handshake

Start the TCP server.

Capture:

```bash
sudo tcpdump -i lo -nn tcp port 5000
```

Connect:

```bash
nc localhost 5000
```

Identify:

```text
SYN
SYN-ACK
ACK
```

Then close the connection and identify FIN/ACK.

## Lab 2 — message framing

Modify your C# app.

Send two logical messages quickly.

Observe that `ReadAsync()` boundaries are not guaranteed to correspond to your `WriteAsync()` calls.

Implement length-prefix framing:

```text
[length][payload]
```

Example:

```text
0005HELLO0005WORLD
```

Then implement:

```csharp
ReadExactlyAsync(...)
```

or equivalent logic.

## Lab 3 — observe states

```bash
ss -tan
```

Look for:

```text
LISTEN
ESTAB
TIME-WAIT
```

## Lab 4 — RST

Terminate a process abruptly while a connection exists and inspect what the peer receives.

## Advanced later

Learn:

- retransmission timeout
- fast retransmit
- slow start
- congestion window

Do not spend weeks implementing TCP.

### Resources

Computer Networking: A Top-Down Approach — Transport Layer chapter

Beej's Guide to Network Programming  
https://beej.us/guide/bgnet/

Wireshark User's Guide  
https://www.wireshark.org/docs/wsug_html/

---

# Stage 5 — UDP

## Topics

Learn:

- connectionless transport
- datagrams
- no delivery guarantee
- no ordering guarantee
- duplicated packets are possible
- application retains message boundaries
- UDP checksum
- when UDP is useful

Compare:

```text
TCP = reliable byte stream
UDP = individual datagrams
```

## Lab

Write:

```text
UdpClient sender
UdpClient receiver
```

Send messages.

Use:

```bash
ss -lunp
sudo tcpdump -i lo -nn udp
```

Then deliberately drop or delay packets later with `tc`.

## Understand use cases

- DNS
- telemetry
- games
- QUIC / HTTP/3
- media

Do not conclude that "UDP = faster TCP." They provide different semantics.

---

# Stage 6 — DNS

## Topics

Learn:

- hostname
- resolver
- stub resolver
- recursive resolver
- authoritative server
- DNS hierarchy
- root
- TLD
- authoritative name server
- TTL
- caching
- negative caching

Important records:

```text
A
AAAA
CNAME
NS
MX
TXT
SRV
```

For backend work, especially understand:

```text
A
AAAA
CNAME
TTL
```

## Linux practice

```bash
dig google.com
dig A google.com
dig AAAA google.com
dig NS google.com
dig +trace google.com
```

Inspect configured resolvers:

```bash
cat /etc/resolv.conf
resolvectl status
```

where available.

## Lab 1 — hosts file

Add a temporary host entry:

```text
127.0.0.1 backend.local
```

Run your server.

Then:

```bash
curl http://backend.local:5000
```

Understand why DNS may not actually have been involved.

## Lab 2 — DNS packet capture

```bash
sudo tcpdump -i any -nn port 53
```

Run:

```bash
dig example.com
```

Inspect request and response.

## Important backend problem

Study the interaction between:

```text
DNS TTL
     +
connection pooling
```

For .NET specifically, understand that DNS is generally resolved when a connection is created; an existing pooled connection can continue being reused.

### Resources

Cloudflare Learning Center — DNS  
https://www.cloudflare.com/learning/dns/what-is-dns/

Microsoft HttpClient guidelines  
https://learn.microsoft.com/dotnet/fundamentals/networking/http/httpclient-guidelines

---

# Stage 7 — HTTP/1.1

## Topics

Understand the wire format.

Request:

```http
GET /users/42 HTTP/1.1
Host: localhost:5000
Accept: application/json
```

Response:

```http
HTTP/1.1 200 OK
Content-Type: application/json
Content-Length: 42
```

Learn:

- request line
- status line
- headers
- request body
- response body
- methods
- status codes
- Host header
- Content-Type
- Content-Length
- Transfer-Encoding
- redirects
- cookies
- caching
- compression
- persistent connections
- idempotency
- conditional requests

## Lab 1 — HTTP manually

Start an ASP.NET server.

Then:

```bash
nc localhost 5000
```

Type:

```http
GET / HTTP/1.1
Host: localhost

```

Observe the raw response.

## Lab 2 — curl

Become comfortable with:

```bash
curl -v http://localhost:5000
curl -I http://localhost:5000
curl -X POST http://localhost:5000/users
curl -H 'Content-Type: application/json' \
     -d '{"name":"Pratyush"}' \
     http://localhost:5000/users
```

Useful diagnostics:

```bash
curl -v
curl --trace -
curl --trace-time -
```

## Lab 3 — implement tiny HTTP yourself

Using your TCP server, recognize:

```text
GET /hello HTTP/1.1
```

and return:

```http
HTTP/1.1 200 OK
Content-Length: 5

hello
```

Do this only as a learning exercise.

## Lab 4 — persistent connection

Send multiple HTTP requests over one connection and observe connection reuse.

### Resources

MDN HTTP overview  
https://developer.mozilla.org/en-US/docs/Web/HTTP/Overview

RFC 9110 — HTTP Semantics  
https://www.rfc-editor.org/rfc/rfc9110

Use the RFC as a reference, not as your first tutorial.

---

# Stage 8 — TLS and HTTPS

## Mental model

```text
HTTP
  ↓
TLS
  ↓
TCP
  ↓
IP
```

## Topics

Learn:

- encryption
- authentication
- integrity
- asymmetric cryptography — conceptual
- symmetric session keys
- certificate
- CA
- certificate chain
- root CA
- intermediate CA
- subject
- SAN
- hostname verification
- SNI
- TLS handshake
- TLS versions
- cipher suites at a high level
- session resumption

## Linux practice

Inspect a server:

```bash
openssl s_client -connect google.com:443
```

With SNI:

```bash
openssl s_client \
    -connect example.com:443 \
    -servername example.com
```

Inspect certificate:

```bash
openssl s_client -connect example.com:443 -servername example.com </dev/null
```

Use curl:

```bash
curl -v https://example.com
```

## Lab

Create a local development certificate and expose your ASP.NET API over HTTPS.

Inspect it with `openssl`.

Try hostname mismatch deliberately.

Understand errors such as:

```text
certificate verify failed
hostname mismatch
certificate expired
unknown CA
```

### Resources

Cloudflare — What is TLS?  
https://www.cloudflare.com/learning/ssl/transport-layer-security-tls/

Mozilla TLS resources  
https://developer.mozilla.org/en-US/docs/Web/Security/Transport_Layer_Security

---

# Stage 9 — HTTP/2

## Topics

Learn:

- binary framing
- streams
- multiplexing
- headers
- HPACK
- stream IDs
- one TCP connection carrying multiple HTTP streams
- HTTP/2 flow control
- TCP-level head-of-line blocking

Mental model:

```text
TCP connection
│
├── HTTP/2 stream 1
├── HTTP/2 stream 3
├── HTTP/2 stream 5
└── HTTP/2 stream 7
```

Compare this with multiple HTTP/1.1 connections.

## Lab

Run an ASP.NET endpoint supporting HTTP/2.

Use:

```bash
curl --http2 -v https://localhost:PORT
```

Inspect protocol negotiation.

Use browser dev tools or Wireshark where useful.

### Resource

HTTP/2 specification / overview  
https://httpwg.org/specs/rfc9113.html

---

# Stage 10 — Polling, long polling, SSE and WebSocket

These belong at the application communication layer.

## 10.1 Polling

Mental model:

```text
client -> GET /status
server -> status

wait

client -> GET /status
server -> status
```

Build a job endpoint:

```text
POST /jobs
GET /jobs/{id}
```

Poll every 2 seconds.

Measure:

- request count
- update latency
- server work

---

## 10.2 Long polling

Client makes a request.

Server waits until data exists or a timeout occurs.

```text
client ----------------------> server
          waiting...
client <------ event -------- server
```

Then the client reconnects.

Lab:

```text
GET /jobs/{id}/wait
```

Hold the request until the job status changes.

---

## 10.3 Server-Sent Events — SSE

Characteristics:

```text
server -> client
```

One long-lived HTTP response.

Learn:

- `text/event-stream`
- `EventSource`
- reconnect
- event IDs
- heartbeat / keepalive considerations
- proxy buffering concerns

Lab:

```text
GET /jobs/{id}/events
```

Stream:

```text
10%
20%
40%
80%
complete
```

Capture the request with:

```bash
curl -N http://localhost:5000/jobs/1/events
```

### Resource

MDN — Using server-sent events  
https://developer.mozilla.org/en-US/docs/Web/API/Server-sent_events/Using_server-sent_events

---

## 10.4 WebSocket

Understand the initial HTTP upgrade:

```http
GET /chat HTTP/1.1
Upgrade: websocket
Connection: Upgrade
```

Then:

```text
client <=================> server
```

Both sides can send messages independently.

Learn:

- opening handshake
- frames
- text vs binary
- ping/pong
- close frames
- reconnect
- heartbeats
- backpressure
- connection state

## Lab

Build a small:

```text
job control channel
```

Client can send:

```text
pause
resume
cancel
```

Server can send progress simultaneously.

Compare the complexity against SSE + normal HTTP commands.

### Resource

MDN WebSocket API  
https://developer.mozilla.org/en-US/docs/Web/API/WebSockets_API

---

# Stage 11 — gRPC

## Topics

Learn:

- Protocol Buffers
- `.proto`
- generated client/server stubs
- service
- RPC
- HTTP/2 transport
- metadata
- status codes
- deadlines
- cancellation

Four RPC forms:

```text
Unary
client -> request
server -> response
```

```text
Server streaming
client -> request
server -> response
       -> response
       -> response
```

```text
Client streaming
client -> request
       -> request
       -> request
server -> response
```

```text
Bidirectional streaming
client <===============> server
```

## C# lab

Create:

```text
JobService
```

Methods:

```protobuf
rpc GetJob(GetJobRequest) returns (Job);
rpc WatchJob(GetJobRequest) returns (stream JobUpdate);
rpc UploadLogs(stream LogLine) returns (UploadResult);
rpc AgentSession(stream AgentMessage) returns (stream ServerMessage);
```

Observe cancellation and deadlines.

Compare this architecture against:

- REST
- SSE
- WebSocket

### Resource

gRPC Core Concepts  
https://grpc.io/docs/what-is-grpc/core-concepts/

ASP.NET Core gRPC  
https://learn.microsoft.com/aspnet/core/grpc/

---

# Stage 12 — Linux network namespaces

This module is extremely valuable before Docker/Kubernetes networking.

## Topics

Learn:

- Linux network namespace
- virtual Ethernet pair
- bridge
- route
- forwarding

Think of a namespace as having its own networking world:

```text
namespace
├── interfaces
├── IP addresses
├── routing table
└── sockets
```

## Lab — create two isolated hosts

Create namespaces:

```bash
sudo ip netns add host1
sudo ip netns add host2
```

Create a virtual Ethernet pair:

```bash
sudo ip link add veth1 type veth peer name veth2
```

Move endpoints:

```bash
sudo ip link set veth1 netns host1
sudo ip link set veth2 netns host2
```

Assign IPs:

```bash
sudo ip netns exec host1 ip addr add 10.0.0.1/24 dev veth1
sudo ip netns exec host2 ip addr add 10.0.0.2/24 dev veth2
```

Bring interfaces up:

```bash
sudo ip netns exec host1 ip link set lo up
sudo ip netns exec host1 ip link set veth1 up

sudo ip netns exec host2 ip link set lo up
sudo ip netns exec host2 ip link set veth2 up
```

Test:

```bash
sudo ip netns exec host1 ping 10.0.0.2
```

Inspect:

```bash
sudo ip netns exec host1 ip addr
sudo ip netns exec host1 ip route
sudo ip netns exec host1 ip neigh
```

Capture packets on the veth.

Once this makes sense, container networking becomes much easier.

---

# Stage 13 — Linux bridges and routing

Build:

```text
        Linux bridge
         /       \
      host1     host2
```

Learn:

- Linux bridge
- L2 switching
- forwarding
- routing between subnets

Create three namespaces if needed:

```text
client ---- router ---- server
```

Give them different subnets and configure forwarding.

Goal:

You should be able to answer:

> Why can these two namespaces not communicate?

by checking:

```text
interface state
IP
subnet
route
forwarding
firewall
```

instead of guessing.

---

# Stage 14 — Linux packet filtering and NAT

Learn modern Linux packet filtering conceptually.

Focus on:

- netfilter
- nftables
- INPUT
- OUTPUT
- FORWARD
- DNAT
- SNAT / masquerade

Do not spend excessive time memorizing firewall syntax.

Understand the packet path.

Useful command:

```bash
sudo nft list ruleset
```

Lab:

Use namespaces to create:

```text
private network -> NAT router -> external namespace
```

Observe source address translation.

This will make Docker networking much clearer.

---

# Stage 15 — Traffic shaping and network failure simulation

Learn Linux `tc`.

This is extremely useful for backend experiments.

## Add latency

Example:

```bash
sudo tc qdisc add dev <interface> root netem delay 200ms
```

## Add packet loss

```bash
sudo tc qdisc change dev <interface> root netem loss 10%
```

## Add jitter

```bash
sudo tc qdisc change dev <interface> root netem delay 100ms 50ms
```

Use these inside test namespaces rather than breaking your main network.

## Lab

Test your HTTP client under:

```text
200ms latency
2% packet loss
10% packet loss
```

Observe:

- latency
- TCP retransmissions
- timeout behavior
- retries

Clean up:

```bash
sudo tc qdisc del dev <interface> root
```

---

# Stage 16 — Reverse proxies

## Topics

Learn:

- forward proxy
- reverse proxy
- HTTP proxying
- upstream
- TLS termination
- forwarded headers
- connection reuse
- buffering
- proxy timeout

## nginx lab

Run:

```text
client
   ↓
nginx :8080
   ↓
ASP.NET :5000
```

Configure a basic reverse proxy.

Inspect:

```text
Host
X-Forwarded-For
X-Forwarded-Proto
```

Then move TLS termination to nginx:

```text
HTTPS
  ↓
nginx
  ↓ HTTP
backend
```

Understand the security implications.

---

# Stage 17 — Load balancing

## Topics

Learn:

- Layer 4 vs Layer 7 load balancing
- round robin
- least connections
- health checks
- passive vs active health checking
- sticky sessions
- connection draining
- TLS termination
- backend connection pools

## Lab

Run:

```text
API :5001
API :5002
API :5003
```

Each returns:

```json
{
  "instance": "server-1"
}
```

nginx:

```text
            ┌-> :5001
client -> LB|-> :5002
            └-> :5003
```

Run:

```bash
for i in {1..20}; do
    curl -s localhost:8080
done
```

Kill one backend.

Observe what happens.

Then add health-check/failure behavior.

---

# Stage 18 — Connection pooling

This is crucial backend networking knowledge.

## Topics

Understand the cost of:

```text
DNS
 ↓
TCP handshake
 ↓
TLS handshake
 ↓
HTTP request
```

versus:

```text
reuse existing connection
 ↓
HTTP request
```

Learn:

- persistent connections
- connection pooling
- pool lifetime
- idle connections
- max connections
- ephemeral ports
- socket exhaustion
- DNS changes with pooled connections

## .NET lab

Compare:

### Bad experiment

Repeatedly create/dispose clients and handlers in a way that creates new connection pools.

### Good experiment

Use:

```text
long-lived HttpClient
```

or:

```text
IHttpClientFactory
```

Observe sockets:

```bash
ss -tan
```

Count TIME_WAIT entries.

Investigate:

```csharp
SocketsHttpHandler
PooledConnectionLifetime
PooledConnectionIdleTimeout
MaxConnectionsPerServer
```

### Resource

Microsoft HttpClient Guidelines  
https://learn.microsoft.com/dotnet/fundamentals/networking/http/httpclient-guidelines

---

# Stage 19 — Timeouts

Do not think of "timeout" as one setting.

Learn:

- DNS timeout
- connection timeout
- TLS handshake timeout
- request timeout
- read timeout
- write timeout
- idle timeout
- application deadline

Mental model:

```text
DNS
 |
 | connect timeout
 v
TCP connection
 |
 | TLS handshake timeout
 v
TLS
 |
 | request/deadline
 v
server processing
 |
 | response/read timeout
 v
response
```

## Lab

Create an API:

```text
GET /delay/{seconds}
```

Call it with different timeout values.

Observe the behavior from both client and server.

---

# Stage 20 — Retries, backoff, jitter and retry storms

## Topics

Learn:

- transient failure
- retries
- exponential backoff
- jitter
- maximum attempts
- retry budget
- idempotency
- retry amplification

Consider:

```text
Service A
   ↓ retry ×3
Service B
   ↓ retry ×3
Service C
```

One original request can produce many downstream attempts.

## Lab — unreliable server

Build an endpoint that randomly:

```text
50% -> HTTP 200
20% -> HTTP 500
20% -> delay
10% -> terminate connection
```

Implement:

```text
timeout
retry
exponential backoff
jitter
cancellation
```

Log every attempt.

Observe the difference between:

```text
immediate retry
```

and:

```text
exponential backoff + jitter
```

---

# Stage 21 — Idempotency

Networking failures make this an important backend concept.

Scenario:

```text
client ------ POST payment ------> server
                               payment succeeds
client <--------- X connection lost
```

The client does not know whether the server processed the request.

If it retries:

```text
POST payment
```

could the payment happen twice?

Learn:

- idempotent methods
- idempotency keys
- deduplication
- request IDs

## Lab

Implement:

```text
POST /orders
Idempotency-Key: abc123
```

Simulate response loss and client retry.

---

# Stage 22 — Observability and network debugging

Your debugging workflow should become systematic.

When:

```text
curl https://service/api
```

is slow, think:

```text
DNS?
    ↓
TCP connect?
    ↓
TLS handshake?
    ↓
load balancer?
    ↓
server processing?
    ↓
downstream call?
    ↓
response transfer?
```

## Essential commands

### DNS

```bash
dig host
```

### routing

```bash
ip route
ip route get <ip>
```

### sockets

```bash
ss -ltnp
ss -tanp
```

### HTTP

```bash
curl -v
```

### TLS

```bash
openssl s_client
```

### packets

```bash
tcpdump
```

### route path

```bash
traceroute
tracepath
```

### throughput

```bash
iperf3
```

## curl timing lab

Use curl timing output to separate:

```text
DNS lookup
connect
TLS
TTFB
total
```

Create a reusable script for this.

---

# Stage 23 — Wireshark

Do not postpone Wireshark until the end.

Use it throughout the syllabus.

Become comfortable with display filters:

```text
tcp
udp
dns
http
tls
tcp.port == 5000
ip.addr == 10.0.0.1
tcp.flags.syn == 1
```

Learn to:

- follow TCP stream
- inspect handshake
- inspect retransmissions
- inspect DNS
- inspect HTTP
- identify FIN/RST
- view packet timings

### Resource

Wireshark User's Guide  
https://www.wireshark.org/docs/wsug_html/

---

# Stage 24 — Docker networking

Only do this after namespaces and bridges.

## Topics

Learn:

- container network namespace
- bridge network
- veth pair
- container IP
- port publishing
- NAT
- DNS-based container discovery
- host networking
- user-defined bridge

Mental model:

```text
container
   |
  veth
   |
docker bridge
   |
host
   |
external network
```

## Labs

Create:

```bash
docker network create backend-net
```

Run two containers.

Communicate using container names instead of IP addresses.

Inspect:

```bash
docker network inspect backend-net
```

Compare:

```text
container IP
host IP
published host port
```

Use:

```bash
ip link
ip addr
sudo nft list ruleset
```

to relate Docker abstractions back to Linux.

### Resource

Docker Networking Overview  
https://docs.docker.com/engine/network/

---

# Stage 25 — Kubernetes networking

Do this only after understanding:

```text
IP
routing
DNS
namespaces
bridges
NAT
Docker networking
```

## Topics

Learn:

### Pod networking

```text
Pod A <-------> Pod B
```

Each Pod gets an IP.

### Service

```text
client
  ↓
Service
  ↓
Pods
```

Learn:

```text
ClusterIP
NodePort
LoadBalancer
```

### Service discovery

Understand:

```text
my-api.default.svc.cluster.local
```

and CoreDNS.

### Gateway / Ingress

```text
Internet
   ↓
Gateway / Ingress
   ↓
Service
   ↓
Pods
```

### Also learn

- EndpointSlice
- kube-proxy at a conceptual level
- CNI
- NetworkPolicy

Do not try to memorize every CNI plugin.

## Labs

Using `kind`, `minikube`, or a development cluster:

1. Deploy a 3-replica API.
2. Create a ClusterIP Service.
3. Call the service from another Pod.
4. Inspect DNS.
5. Delete a backend Pod while sending traffic.
6. Expose via Ingress/Gateway.
7. Add a NetworkPolicy and deliberately block communication.

### Resources

Kubernetes Services, Load Balancing, and Networking  
https://kubernetes.io/docs/concepts/services-networking/

Kubernetes Service documentation  
https://kubernetes.io/docs/concepts/services-networking/service/

---

# Stage 26 — HTTP/3 and QUIC

Treat this as later material.

Mental model:

```text
HTTP/1.1 -> TCP
HTTP/2   -> TCP
HTTP/3   -> QUIC -> UDP
```

Learn:

- QUIC
- UDP transport
- built-in TLS
- streams
- connection migration
- avoiding TCP-level head-of-line blocking between streams

Do not attempt to implement QUIC yourself.

---

# Stage 27 — Backend communication patterns comparison

At this point you should be able to choose between mechanisms based on semantics rather than fashion.

| Mechanism | Direction | Persistence | Typical use |
|---|---|---|---|
| HTTP request/response | client → server → client | short/reused connection | APIs |
| Polling | repeated request/response | repeated | simple status |
| Long polling | server delays response | repeated long requests | event notification |
| SSE | server → client | long lived | progress/logs/events |
| WebSocket | bidirectional | long lived | interactive real-time |
| gRPC unary | request/response | pooled HTTP/2 | service-to-service |
| gRPC streaming | one/bidirectional streams | long lived | backend streaming |
| Kafka | producer/log/consumer | durable log | asynchronous durable events |

Questions to ask:

```text
Who initiates communication?

Does communication need to be bidirectional?

Can events be lost?

Must clients reconnect and resume?

Do events need durable storage?

How many concurrent connections exist?

Can proxies/load balancers handle the protocol?

What happens during a server restart?
```

---

# Stage 28 — Capstone: NetworkLab

Create one repository:

```text
NetworkLab/
│
├── 01-ip-routing/
├── 02-tcp-echo/
├── 03-tcp-framing/
├── 04-udp/
├── 05-dns/
├── 06-http-from-scratch/
├── 07-tls/
├── 08-http2/
├── 09-polling/
├── 10-long-polling/
├── 11-sse/
├── 12-websocket/
├── 13-grpc/
├── 14-linux-namespaces/
├── 15-linux-routing/
├── 16-network-failure-simulation/
├── 17-nginx-reverse-proxy/
├── 18-load-balancer/
├── 19-httpclient-pooling/
├── 20-timeout-retry/
├── 21-docker-networking/
└── 22-kubernetes-networking/
```

Every folder should contain:

```text
README.md
source code
commands.md
observations.md
```

For packet-oriented labs, optionally keep:

```text
captures/
```

Do not commit huge `.pcap` files unnecessarily.

---

# Suggested learning schedule

This is intentionally not a rigid calendar. Move forward when you can explain the previous stage.

## Block A — Core networking

```text
1. Networking model
2. IP + routing
3. sockets
4. TCP
5. UDP
6. Wireshark/tcpdump
```

Spend the most time on TCP.

---

## Block B — Web backend networking

```text
7. DNS
8. HTTP/1.1
9. TLS
10. HTTP/2
```

At the end you should be able to explain:

```text
curl https://api.example.com/users
```

from DNS resolution through receiving the HTTP response.

---

## Block C — Streaming

```text
11. polling
12. long polling
13. SSE
14. WebSocket
15. gRPC streaming
```

Build the same job-progress application using multiple mechanisms.

That comparison is the learning exercise.

---

## Block D — Production backend behavior

```text
16. connection pooling
17. timeouts
18. retries
19. backoff + jitter
20. idempotency
21. reverse proxy
22. load balancing
```

---

## Block E — Linux networking

```text
23. namespaces
24. veth
25. bridges
26. routing
27. NAT
28. nftables
29. tc/netem
```

This is where networking becomes much less abstract.

---

## Block F — Infrastructure

```text
30. Docker networking
31. Kubernetes networking
32. HTTP/3 + QUIC overview
```

---

# Recommended resource strategy

Do not try to finish five networking books.

Use one primary conceptual source plus official references.

## Primary conceptual source

**Computer Networking: A Top-Down Approach — Kurose & Ross**

Recommended sections for backend engineering:

```text
Application Layer
Transport Layer
Network Layer
selected Link Layer concepts
network security / TLS-related material
```

You can deprioritize deep wireless/mobile material initially.

---

## Socket programming

**Beej's Guide to Network Programming**

https://beej.us/guide/bgnet/

---

## Linux networking

Linux man pages / iproute2:

https://man7.org/linux/man-pages/man8/ip.8.html

https://man7.org/linux/man-pages/man8/ip-route.8.html

https://man7.org/linux/man-pages/man8/ss.8.html

The local man pages are also valuable:

```bash
man ip
man ip-route
man ss
man tcp
man udp
man socket
```

---

## Packet analysis

**Wireshark User's Guide**

https://www.wireshark.org/docs/wsug_html/

---

## HTTP

MDN HTTP:

https://developer.mozilla.org/en-US/docs/Web/HTTP

HTTP specifications:

https://httpwg.org/specs/

---

## TLS

MDN TLS:

https://developer.mozilla.org/en-US/docs/Web/Security/Transport_Layer_Security

Cloudflare learning material:

https://www.cloudflare.com/learning/ssl/transport-layer-security-tls/

---

## SSE

MDN:

https://developer.mozilla.org/en-US/docs/Web/API/Server-sent_events/Using_server-sent_events

---

## WebSocket

MDN:

https://developer.mozilla.org/en-US/docs/Web/API/WebSockets_API

---

## gRPC

Official gRPC documentation:

https://grpc.io/docs/what-is-grpc/core-concepts/

.NET gRPC:

https://learn.microsoft.com/aspnet/core/grpc/

---

## .NET HTTP networking

HttpClient guidelines:

https://learn.microsoft.com/dotnet/fundamentals/networking/http/httpclient-guidelines

Study:

```text
HttpClient
SocketsHttpHandler
IHttpClientFactory
connection pooling
DNS refresh
PooledConnectionLifetime
MaxConnectionsPerServer
```

---

## Docker

Docker Networking:

https://docs.docker.com/engine/network/

---

## Kubernetes

Networking concepts:

https://kubernetes.io/docs/concepts/services-networking/

Services:

https://kubernetes.io/docs/concepts/services-networking/service/

---

# Linux command checklist

By the end, these should feel normal.

## Interfaces

```bash
ip addr
ip link
```

## Routes

```bash
ip route
ip route get 8.8.8.8
```

## Neighbours

```bash
ip neigh
```

## Sockets

```bash
ss -ltnp
ss -tan
ss -lunp
ss -s
```

## DNS

```bash
dig
dig +trace
resolvectl
```

## HTTP

```bash
curl
curl -v
curl -I
```

## Raw connections

```bash
nc
```

## Packets

```bash
tcpdump
```

## TLS

```bash
openssl s_client
```

## Route debugging

```bash
ping
traceroute
tracepath
```

## Performance

```bash
iperf3
```

## Namespaces

```bash
ip netns
```

## Traffic manipulation

```bash
tc
```

## Firewall/NAT

```bash
nft
```

---

# Backend networking debugging checklist

When a service cannot reach another service, debug in roughly this order.

```text
1. Is the hostname correct?
2. Does DNS resolve?
3. What IP was returned?
4. Is there a route to that IP?
5. Is the destination process listening?
6. Is the port correct?
7. Can TCP connect?
8. Does TLS succeed?
9. Does HTTP/gRPC succeed?
10. Is a proxy/LB changing behavior?
11. Is there a timeout?
12. Is there packet loss or latency?
13. Is connection pooling holding stale connections?
14. Is a firewall/security policy blocking it?
15. Are retries hiding the original error?
```

Useful commands:

```bash
dig service.example.com

ip route get <resolved-ip>

ss -ltnp

nc -vz host port

curl -v https://host

openssl s_client -connect host:443 -servername host

sudo tcpdump -i any host <ip>
```

---

# What you can skip initially

For a backend engineer, you do not need deep expertise initially in:

- BGP configuration
- OSPF administration
- spanning tree protocol configuration
- enterprise VLAN design
- MPLS
- wireless PHY details
- router vendor CLI configuration
- carrier networking
- deep Ethernet switching internals

Learn them later if your work requires them.

You **should** know enough routing and subnetting to understand cloud and container networks.

---

# Final competency target

You are done with the first serious pass when you can explain this end-to-end:

```text
C# code
    |
HttpClient
    |
DNS resolution
    |
socket
    |
TCP handshake
    |
TLS handshake
    |
HTTP/2
    |
local routing table
    |
network interface
    |
router / NAT
    |
load balancer
    |
reverse proxy
    |
Kubernetes Service
    |
Pod
    |
ASP.NET server
    |
downstream DB/service
```

and, when something in that chain fails, you know which Linux tool can give you evidence.

That is the level of networking knowledge that pays off directly in backend engineering.

---

# 📓 My Learning Log

> Running notes from hands-on sessions. Resume from **"Next step"** at the bottom of the latest session.

## Session 1 — 2026-10-06 — First Wireshark captures (Windows, no Docker yet)

**Setup:** Wireshark + Npcap on Windows. Captured on **Wi-Fi** (internet traffic) and **Adapter for loopback traffic capture** (`localhost` traffic).

### Mental model
- **Packet** = envelope. **IP address** = building address. **Port** = apartment number (which program).
- **DNS** = phonebook (name → IP). **TCP** = phone call (handshake, reliable, ordered). **UDP** = postcard (no handshake, no guarantee).
- Frame on Wi-Fi = Ethernet (14) + IP (20) + TCP (20) = **54 bytes of headers**; data length = `TCP Segment Len` in the middle pane (more reliable than subtracting).
- **Filtering is the core Wireshark skill** — networks are always noisy (e.g. adb, Firefox chatter on loopback).

### Experiment 1 — DNS (`nslookup google.com`, filter `dns`)
- Me = `192.168.0.168`, DNS server = router `192.168.0.1`; round trip ≈ 2.6 ms.
- **A** = IPv4 address, **AAAA** = IPv6 address — clients ask for both.
- **PTR** = reverse lookup (IP → name); nslookup looks up the DNS server's own name → "No such name" (harmless).
- DNS uses **UDP port 53**; query/response matched by **transaction ID** (`0x0002` ↔ `0x0002`).
- `nslookup` asked A then AAAA sequentially; `curl.exe` asked both **in parallel**.

### Experiment 2 — TCP connection (`curl.exe http://example.com`, filter `tcp.port == 80`)
- **Handshake:** `SYN` → `SYN, ACK` → `ACK`. Client sends data right after its ACK (ACKs are never ACKed).
- **TCP ACK ≠ application response:** server's OS ACKs the request in ~12 ms; the web app replies ~40 ms later. Different layers.
- **301 Moved Permanently** → `Location: http://www.google.com/` → new DNS lookup (8 IPs = DNS load balancing) + new TCP connection.
- ⚠️ In Windows PowerShell 5.1, `curl` = `Invoke-WebRequest` (follows redirects, keeps alive). Real curl = **`curl.exe`**.
- `Connection: Keep-Alive` keeps the connection open for reuse → no FIN until idle timeout.
- **Close:** `FIN,ACK` → `FIN,ACK` → `ACK` (3 packets when the other side merges its ACK+FIN; textbook is 4). Two one-way lanes, each closed separately.
- Side that sends **FIN first** goes into **TIME_WAIT** (`Get-NetTCPConnection -State TimeWait`). Too many new connections → TIME_WAIT pile-up → port exhaustion → why keep-alive / connection pooling exist (Stage 18).

### Seq / Ack — my understanding ✅
- **Seq** = byte number this packet's data **starts** at (own direction's counter).
- **Ack** = "I've received everything before this number; send me this byte next."
- Two independent counters, one per direction.
- **Next Seq = Seq + Len (+1 for SYN or FIN)**; **Ack = other side's next Seq**.
- **Cumulative ACK:** one ACK can confirm many packets.
- **Ack never jumps over a gap.** Loss of a middle packet → receiver keeps sending the same Ack (**Dup ACK**) → sender **retransmits** → Ack jumps forward over everything buffered.
- `SACK_PERM` in SYN = Selective ACK allowed (can report "I have 2001–3000 but miss 1001–2000").
- Wireshark shows **relative** Seq (starts at 0); real/raw Seq starts at a random number.
- Worked example (example.com): GET Len=75 → Ack=76; response 931+5 bytes → Ack=937; FINs: 76→77, 937→938.

### Experiment 3 — Connection timeout (`curl.exe http://google.com:81 --connect-timeout 10`)
- Only my `SYN` packets, **no reply** → `[TCP Retransmission]` of same SYN (same port, Seq=0).
- Gaps **1 s → 2 s → 4 s** = **exponential backoff**; curl gave up at 10 s.
- Silence = packet **dropped** (firewall, wrong IP, host down). **AWS Security Groups drop silently → timeouts.**
- Always set connect timeouts in code (OS defaults: ~20 s Windows, ~2 min Linux) — Stage 19.

### Experiment 4 — Connection refused (`curl.exe http://localhost:9999`, loopback adapter, filter `tcp.port == 9999`)
- Every `SYN` answered by **`RST, ACK`** within ~16 µs (`Ack=1` = SYN counted as 1 byte).
- **Happy Eyeballs:** curl tried `::1` (IPv6) first, then `127.0.0.1` (IPv4) 200 ms later, in parallel.
- **Windows quirk:** retries SYN after RST (5×, 0.5 s apart) → "refused" takes ~2 s. Linux fails instantly.
- Loopback `MSS=65475` vs Wi-Fi `MSS=1460` (no physical link limit) — MTU/MSS later.

| | **Timeout** | **Refused** |
|---|---|---|
| Reply to SYN | nothing | RST immediately |
| Meaning | dropped: firewall / SG / routing / host down | host reached, nothing listening |
| Check first | firewall, Security Group, IP, route | is the app running? right port? |

### Useful commands learned
```powershell
nslookup example.com
curl.exe http://example.com
curl.exe http://google.com:81 --connect-timeout 10
Get-NetTCPConnection -State TimeWait
Get-NetTCPConnection -State Established | Where-Object LocalAddress -eq '127.0.0.1'   # Linux: ss -tnp
```

### Wireshark features used
Display filters (`dns`, `tcp.port == 80`) · Follow → TCP Stream · Statistics → Conversations · TCP section in middle pane (`Sequence Number`, `[Next Sequence Number]`, `TCP Segment Len`, `Sequence Number (raw)`).

### Syllabus coverage
Roughly **Stages 1, 4, 5, 6** (basics) + early Stage 23 (Wireshark).

### ▶️ Next step (superseded — see "🔁 Approach review" below)
1. Be the server: `python -m http.server 9999`, then `curl.exe http://localhost:9999` with filter `tcp.port == 9999`. Verify Seq/Ack math and see who sends FIN first.
2. Then set up **WSL2 Ubuntu + Docker Engine** and capture with `nicolaka/netshoot` + tcpdump → `.pcap` → Wireshark.
3. Break things on purpose: `tc netem` packet loss (see real Dup ACKs / retransmissions), firewall drops, nginx load balancer.

## 🔁 Approach review — 2026-10-06

### What's going well
- Session 1 was the right kind of learning: real packets, real failures (timeout vs refused), and the Seq/Ack math got worked through properly.

### What to change
1. **Biggest gap: Layer 3 (IP, subnets, routing, ARP, NAT) hasn't been touched.** Everything so far is TCP/DNS from one laptop to the internet. Docker, Kubernetes and AWS VPC networking are mostly Layer 3 plus Linux plumbing (namespaces, veth, bridges, iptables). Without it, K8s/VPC turns into memorizing terms.
2. **Move to Linux now.** WSL2 Ubuntu and Docker Desktop are already installed. Windows quirks (SYN retries after RST, the `curl` alias) are noise. Servers, containers, K8s nodes and EC2 are Linux.
3. **Predict before you run.** Write down what you expect *before* every command or capture. A wrong prediction is where the learning is. Watching output without predicting feels like learning but doesn't stick.
4. **Start every session from memory.** Spend 10 minutes answering questions on the previous session with notes closed, then check.
5. **End every session by breaking something and diagnosing it.** That's the skill the job actually needs.
6. **Write this log yourself, in your own words.** Ask Claude to *check* your explanation, not to write it. If you can't write it, you don't know it yet.

### Re-sequenced roadmap (goal: K8s platforms + AWS infra)
| Phase | What | Syllabus stages | Status |
|---|---|---|---|
| A | TCP / DNS from the client seat | 1, 4, 5, 6 (basics), 23 | ✅ mostly done |
| **B** | **Layer 3 in Linux: IP, subnets, routes, ARP, NAT → build "containers" by hand (netns, veth, bridge, iptables) → compare with real Docker** | 2, 12, 13, 14, 24 | ▶️ **now** (2026-10-09): 2, 12, 13 done; 14 done except DNAT; 24 next. Plan: `NetworkLab/02-virtual-network/NEXT.md` |
| C | Backend traffic in docker compose: HTTP, TLS, nginx proxy/LB, timeouts/retries/pooling under `tc netem` loss | 7, 8, 15–20, 22 | |
| D | Kubernetes with `kind`: pod IPs, Service = iptables DNAT, CoreDNS, ndots | 25 | |
| E | AWS VPC: map every piece back to Phase B (route table, IGW, NAT GW, SG = stateful filter) | — | |
| later | HTTP/2, SSE/WebSocket, gRPC, QUIC: learn these when a project needs them | 9, 10, 11, 26 | deferred |

### ▶️ Session 2 — "Follow one ping through two NATs" (WSL Ubuntu)
Your setup: WSL `eth0` = `172.26.169.218/20`, gateway `172.26.160.1` (Windows) → Wi-Fi `192.168.0.168` → router `192.168.0.1` → ISP. That's **two NATs**, and you'll get to see both.

**Before starting, read this (15 min, just enough theory):**
- **IP address** = a street address for a device, e.g. `192.168.0.168`.
- **Prefix (`/24`, `/20`)** = how much of the address is the "street name" and how much is the "house number". Two devices on the same street can talk to each other directly.
- **Gateway / router** = the door out of your street. Anything for another street goes to the gateway.
- **Routing table** = a list of rules: "for these addresses, go out this interface (via this gateway)".
- **MAC address** = the name of the device sitting next to you on the same cable or Wi-Fi. **ARP** = shouting "who has 172.26.160.1? tell me your MAC".
- **Private IPs** (`10.x`, `172.16–31.x`, `192.168.x`) aren't allowed on the internet. **NAT** = the router swaps your private source address for its own public one and remembers to swap it back on the reply, like an office receptionist.
- **ping** = an "are you there?" packet (ICMP). It has no ports.
- Stuck on a word? Ask Claude mid-session. Guessing a prediction wrong is fine; skipping the prediction isn't.

0. **Setup:** `sudo apt install -y tcpdump dnsutils`
1. **Warm-up (no notes):** (a) Which packets do you see on a timeout, and which on a refused connection? (b) The client sends Seq=1, Len=75. What Ack comes back? (c) Which side ends up in TIME_WAIT? (d) What does a Dup ACK tell you? *(Optional close-out of Phase A: run `NetworkLab/01-tiny-server/server.ps1`. **Predict** who sends FIN first and which side gets TIME_WAIT.)*
2. **Predict, then run:** `ip -br addr`, `ip route`, `ip route get 8.8.8.8`, `ip route get 172.26.170.5`, `ip route get 127.0.0.1`. For each one, write down the interface and whether there's a `via` *before* you run it. *(Subnet math such as the range of `172.26.160.0/20` is optional for now; it gets its own session later.)*
3. **ARP:** `sudo ip neigh flush dev eth0`. In terminal 1 run `sudo tcpdump -n -e -i eth0 arp or icmp`, and in terminal 2 run `ping -c 3 8.8.8.8`. **Predict:** which IP does the ARP request ask about?
4. **See NAT:** run the same ping while Wireshark captures on Windows **Wi-Fi** (filter `icmp`), and on **vEthernet (WSL)** if it's listed. Compare the source IPs at each point. What source IP does 8.8.8.8 see? (`curl.exe ifconfig.me`). Draw the chain.
5. **Break it:** `sudo ip route del default`, then `ping 8.8.8.8` and `ping 172.26.160.1`. Do they fail the same way? Is any packet sent at all? Add a **third row** to your failure table next to timeout and refused. Restore with `sudo ip route add default via 172.26.160.1` (or `wsl --shutdown`).
6. **Explain back:** write 5 sentences in your own words: connected route vs `via`, why ARP never asks for a remote IP, and what NAT rewrote. → AWS link: the VPC "local" route = connected route; `0.0.0.0/0 → igw/nat` = default route; deleting it = a private subnet with no NAT gateway.

**Next after that:** Session 3 builds two "containers" with `ip netns` + a veth pair and pings between them. Session 4 adds a bridge + `MASQUERADE` so they can reach the internet, then you find the same pieces inside real Docker (`docker0`, veths, `iptables -t nat -L`).

## 📚 Reading list (skim alongside experiments)

> Rule: **experiment → capture → read the matching section → explain it back.** Don't read ahead of the labs.

**Primary (one book only):** *Computer Networking: A Top-Down Approach* — Kurose & Ross (+ free Kurose video lectures)
- [ ] Ch 1 — Introduction (packets, delay, layers)
- [ ] Ch 2 — Application Layer: HTTP + DNS sections (↔ Experiments 1–2)
- [ ] Ch 3 — Transport Layer: UDP + TCP sections (↔ Seq/Ack, retransmission, handshake/close)
- [ ] Ch 4 — Network Layer: IPv4 addressing/subnets, forwarding, NAT (↔ Phase B, Session 2)
- [ ] Ch 6 — Link Layer: ARP section only (↔ Session 2 step 3)

**Step-back reading after Session 3 (2026-10-07), in this order:**
- [ ] Practical Networking (Ed Harmoush, YouTube / practicalnetworking.net): "Packet Traveling" series (↔ Sessions 2–3: ARP, MAC vs IP, routers)
- [ ] Beej's Guide to Network Concepts (https://beej.us/guide/bgnet0/): IP, subnet masks, routing, ARP, NAT chapters
- [ ] Kurose & Ross Ch 4: IPv4 addressing (CIDR / prefixes) + NAT · Ch 6: link-layer addressing and ARP
- [ ] `man ip-route`, `man ip-netns`, `man ip-link` (veth section)
- [ ] Red Hat Developer: "Introduction to Linux interfaces for virtual networking" (veth / bridge / tun sections only)
- [ ] iximiuz: "Container Networking Is Simple!" (https://iximiuz.com/en/posts/container-networking-is-simple/): read up to and including the veth part; the bridge/NAT part after Session 4

**Companions:**
- [ ] *Practical Packet Analysis* — Chris Sanders — Wireshark basics + TCP/UDP chapters
- [ ] *High Performance Browser Networking* — Ilya Grigorik (free: https://hpbn.co) — Ch 2 (TCP) now; TLS/HTTP2 later

**Light / 10-minute:**
- Julia Evans zines — *Networking! ACK!*, *How DNS Works* (https://wizardzines.com)
- YouTube — Chris Greer (Wireshark/TCP analysis), Hussein Nasser (backend: TCP, proxies, load balancers)

**Later (don't open yet):**
- *TCP/IP Illustrated, Vol. 1* — Stevens/Fall (deep reference)
- *Networking and Kubernetes* — Strong & Lancey (O'Reilly) — at Stage 25
- AWS VPC documentation — cloud stage


## My understanding
- A computer has network interfaces. IP addresses has attached to these interfaces. Each interface also has it's mac address.
- we can see the interfaces via ip addr 
- with ip route we get the route tables that is how a particular IP will pass through. for example for all the ip inside a subnet can go directly without hitting a gateway.
- in my wsl ubuntu the ip address assigned to interface eth0 is managed by windows. windows has created a subnet 172.26.160.0/20 (my address 172.26.169.218 is one host inside it) that means in 32 bits of ipv4 address 20 bits is the network address and others are host. with in this subnet there is the gateway 172.26.160.1 through which packets with destination ip address outside of the subnet goes via this.
- in a local network from the IP address we need to know the mac address first so that the frame is moved to that device. for that during the request initiation ARP request is sent by the kernel TCP/IP implementation software. the request is a broadcast (ff:ff:ff:ff:ff:ff), the reply comes back only to the asker. you can think of ARP request as Frames-> ARP. ARP is sibling to IP packets. IP packets can contain ICMP or TCP or UDP  packets.
- The frame always goes to the MAC of the **next hop**: the destination itself if it is in my subnet, otherwise the gateway. e.g. a DNS query to a DNS server outside my subnet goes in a frame to the gateway's MAC. In WSL the DNS server is 10.255.255.254 on `lo`, so no ARP and no frame on the network at all → order was DNS → ARP (for gateway) → SYN.
- If you see the idea of frame is to identify the mac address in the current subnet either gateway or  any device. so a frame lives only on one local network (one hop) — true everywhere, even on public internet links. **local** = one street / one hop (frames, MAC). **private** = IP addresses not allowed on the internet (10.x, 172.16–31.x, 192.168.x) → needs NAT.
- Frame contains the IP packet where the source IP and destination IP is mentioned. every sender (my Ubuntu too, and then every router at each hop) inspects the destination IP: if the destination IP is inside the current subnet it does ARP to get the MAC of the destination machine. if not in the same subnet through ARP it gets the mac of the gateway. and sends this frame.
- ip route get 8.8.8.8 tells which route it used in the route table
- ip addr tells the interfaces
- ip neigh keeps the ARP table that is the IP to mac address , can think of a cache so that we don't need to do ARP everytime 
- `lo` (127.0.0.1) is the door back into the same machine: no ARP, nothing leaves the machine. Ubuntu's localhost and Windows' localhost are different.
- NAT rewrites the private source IP to the router's own IP and remembers it in a NAT table (ports for TCP/UDP, ICMP id for ping) so it can swap it back on the reply. My chain: Windows (NAT #1: 172.26.169.218 → 192.168.0.168) → home router (NAT #2 → public IP) → maybe ISP CGNAT (NAT #3). Normal routers don't rewrite IPs — they only lower TTL and change MACs.
- NAT vs ARP: NAT answers "which **address**?", ARP answers "which **face** (MAC)?". Both are needed — the NAT table gives the inside IP, ARP still finds that device's MAC.
- Blocking ICMP stops `ping` but not `curl` (TCP). "Ping fails" ≠ "server down" — test the real port (`curl`, `nc -vz host 80`).

## Session 3 — 2026-10-07 — Building a "container" by hand (network namespace + veth)

### Why namespaces (my Phase 1 answers)
- Two programs on port 80 → "address already in use". An untrusted app shouldn't see all my interfaces/routes. Docker runs many containers that each think they have their own `eth0` → need **isolated copies of the network**.
- **Network namespace** = a separate copy of the network stack: its own interfaces, route tables, ARP (neighbour) table, firewall rules **and ports/sockets** (that's why two containers can both listen on :80). It isolates only the network — files and processes are still shared (prompt stayed in `/mnt/c/Users/praty`).

### What an interface really is
- An interface = kernel object for "a place packets go out / come in". Behind each one is a **driver**.
- Steps before the driver are the same for every interface: socket → IP header → route lookup (`ip route`) → neighbour lookup (`ip neigh`/ARP) → Ethernet frame → queue (`qdisc`, tcpdump copies here) → driver "transmit this frame".
- Only the driver differs:
  - real NIC → copies to the card → electrical/radio signals
  - **`hv_netvsc`** (my WSL `eth0`) → Hyper-V virtual NIC → shared-memory channel (VMBus) to Windows. Ubuntu never touches the real Wi-Fi card; only Windows does.
  - `lo` → hands it straight back to the same kernel
  - **veth** → hands it to its peer, which receives it as if it arrived
- Checked with `ls -l /sys/class/net/` and `basename $(readlink /sys/class/net/eth0/device/driver)` → `hv_netvsc`.

### Lab steps and what I saw
| Step | Command | Result |
|---|---|---|
| Baseline | `ip -br addr`, `ip route`, `ip netns list` | `lo`, `eth0`; 2 routes; no namespaces |
| Create box | `sudo ip netns add box1` → `sudo ip netns exec box1 bash` | only `lo`, **DOWN**, no address, **0 routes** (all predicted ✅) |
| Ping in empty box | `ping -c 1 127.0.0.1` | **Network is unreachable** — instant, no packet sent, no route matches |
| `lo` up | `ip link set lo up` | `127.0.0.1` appears, `ip route` still empty, ping works ✅ |
| Cable | `ip link add veth-host type veth peer name veth-box` | both ends appear in Ubuntu, DOWN, `M-DOWN` (= my peer is down), each has its own random MAC |
| Plug in | `ip link set veth-box netns box1` | `veth-box` moves (same MAC `32:59:…`), Ubuntu shows `veth-host@if3` — after `@` = the peer; Ubuntu can't see box1's names, only the interface number |
| Addresses | `10.0.0.1/24` on `veth-host`, `10.0.0.2/24` on `veth-box`, both `up` | auto routes `10.0.0.0/24 dev veth-host` / `dev veth-box` (`proto kernel`), `local 10.0.0.1` + `broadcast 10.0.0.255` in Ubuntu's local table (all predicted ✅) |

### Hidden routing tables
- `ip route` shows only the **main** table. The **local** table (`ip route show table local`) holds "is this **me**?" addresses: my own IPs (`127.0.0.1`, `172.26.169.218`, `10.0.0.1`) + broadcast addresses. Filled automatically by the kernel.
- `ip rule` = which tables are checked, in order: `0: local` → `32766: main` → `32767: default`. First "is it for me?", then "where do I send it?". (Rules can also pick tables by source = policy routing — later: multi-NIC EC2, VPNs, K8s CNIs.)
- My own `172.26.169.218` matches both `local` (me) and `172.26.160.0/20` (neighbour) — local is checked first, so it wins. Sending to my own IP never leaves through eth0.

### Route matching
- `10.0.0.0/24` = first 24 bits (first 3 numbers) must match → covers `10.0.0.0`–`10.0.0.255`. A route is built from **my own address + prefix**, not from the neighbour's address.
- Several rules can match; **longest prefix match** wins. `default` = `0.0.0.0/0` matches everything but is least specific → only used when nothing else matches.

### Ping Ubuntu → box1 (`tcpdump -n -e -i veth-host`, `ping -c 3 10.0.0.2`)
1. ARP request `5e:db… > ff:ff:ff:ff:ff:ff` "who-has 10.0.0.2 tell 10.0.0.1" — broadcast because I don't know box1's MAC yet.
2. ARP reply sent **only to the asker** — box1 already learned Ubuntu's IP+MAC from the request.
3. `5e:db:dc:a8:c3:e2` = Ubuntu's `veth-host`, `32:59:2c:37:1a:79` = box1's `veth-box` (the ARP reply labels it: `10.0.0.2 is-at 32:59…`).
4. **No NAT** — no router in between and nobody configured a rewrite; NAT only happens at a router told to rewrite (usually the edge to the internet).
5. RTT ≈ **0.19 ms** vs Windows gateway < 1 ms vs Google ~10 ms → veth just hands the frame to its peer in memory; every extra hop / real hardware adds time.
6. ~5 s later box1 sends a **unicast** ARP "who-has 10.0.0.1" → verifying the MAC it learned passively. Linux stores passively learned entries as **STALE**, confirms them when used → **REACHABLE** (`ip neigh` shows the state). Same thing the gateway did in Session 2.
- Last line `ICMP6 router solicitation` → IPv6 auto-asks "is there an IPv6 router here?" when an interface comes up; noise. `fe80::…` addresses are automatic IPv6 link-local addresses built from the MAC.

### Failure table so far
| Error | Meaning |
|---|---|
| Timeout | packets dropped on the way (firewall, Security Group, host down) |
| Refused (RST) | reached the host, nothing listening on that port |
| **Network is unreachable** | **my own machine** has no matching route — no packet is even sent |

### ▶️ Next step
1. Route quiz: predict then `ip route get 10.0.0.200` and `ip route get 10.0.1.5`.
2. Part F: predict, then `sudo ip netns exec box1 ping -c 2 8.8.8.8` with tcpdump on `veth-host`.
3. Phase 5: draw Ubuntu ↔ box1 from memory, then design what box1 is missing to reach the internet → Session 4 (bridge + default route + NAT/MASQUERADE, then compare with real Docker).
4. Cleanup when done: `sudo ip netns del box1`.

### My Learning till now

#### 1. The general idea of networking
- Networking moves data from computer A to computer B. A and B can connect directly or through other devices.
- Each layer wraps the data in a header. This is encapsulation.
- The transport header (TCP or UDP) has the source port and the destination port. A port identifies the process.
- The IP header has the source IP and the destination IP. These identify the two end hosts.
- The Ethernet header has the source MAC and the destination MAC. These identify the two ends of the current link only.
- Models such as OSI and TCP/IP divide this process into layers.

#### 2. IP addresses
- An IP address is a number on an interface. The interface uses it to communicate with the Internet Protocol.
- An IP address belongs to an interface, not to a host. A host with two interfaces can have two IP addresses.
- There are two versions:
  - IPv4 has 32 bits. We write it as four decimal numbers with dots. Example: `10.0.0.1`.
  - IPv6 has 128 bits. We write it in hexadecimal. Example: `fe80::1`.
- Public addresses are for use on the Internet.
- Private addresses are for use in private networks only (RFC 1918):
  - `10.0.0.0/8`
  - `172.16.0.0/12`
  - `192.168.0.0/16`
- The `/8` is the prefix length (CIDR notation). It tells that the first 8 bits are the network bits. The remaining 32 - 8 = 24 bits are the host bits.
- The subnet mask for `/8` is `255.0.0.0`. The prefix length and the mask give the same information.
- In `10.0.0.0/8`, the network address is `10.0.0.0`. The broadcast address is `10.255.255.255`.
- Do not give the network address or the broadcast address to a host.

#### 3. MAC addresses
- A MAC address is a 48-bit label on an interface.
- A MAC address identifies the interface on the local link only.
- An IP address identifies the interface everywhere, on the local link and across networks.
- Data moves hop by hop. Each device sends the data to the next hop only.
- The IP addresses tell the end-to-end source and destination.
- The MAC addresses tell the source and destination of the current hop.

#### 4. TCP and UDP
- TCP and UDP are transport protocols. Both use ports to identify the process.
- TCP:
  - TCP divides the byte stream into segments.
  - TCP delivers the bytes in order.
  - TCP cannot prevent packet loss. TCP finds lost segments and sends them again (retransmission).
  - TCP has flow control and congestion control.
  - Sequence numbers, ACKs and windows give these features.
- UDP:
  - UDP does not divide the data. UDP sends one datagram for each send call.
  - UDP has no handshake, no retransmission and no ordering.
  - UDP has less overhead. Thus, UDP is faster to start and lighter.
- The TCP three-way handshake:
  1. The client sends SYN with a random sequence number. Wireshark shows it as relative seq 0.
  2. The server sends SYN-ACK with its own random sequence number (relative seq 0) and ack 1.
  3. The client sends ACK with seq 1 and ack 1. Then the client starts to send data.
- A sequence number counts bytes, not packets.
- The ACK number is the next byte that the receiver expects.
- A SYN uses one sequence number. Thus, the reply to seq 0 is ack 1.
- The handshake only syncs the start sequence numbers of the two sides.

#### 5. How to do subnetting (VLSM)
1. For each network, count the hosts.
2. Add 1 for the gateway if the network connects to a router.
3. Add 2 for the network address and the broadcast address.
4. Find the smallest n where the total ≤ 2^n. The prefix length is 32 - n.
5. Sort the networks by size, from the largest to the smallest.
6. Give addresses to the largest network first.
7. Start each block at a multiple of its own size. Example: a /26 can start at .0, .64, .128 or .192. It cannot start at .32.
- A link between two routers needs a /30 (2 usable addresses). A /31 also works on a point-to-point link (RFC 3021).

#### 6. Linux network namespace
- A network namespace is an isolated instance of the network stack.
- It has its own:
  - interfaces
  - route tables
  - ARP (neighbor) table
  - iptables rules
  - sockets and ports
  - sysctls, such as `ip_forward`
- A new namespace does not copy the host stack. It starts empty. It has only `lo`, and `lo` is down.

#### 7. veth pair
- A veth pair is a virtual Ethernet cable with two ends. Each end is an interface.
- A frame that goes into one end comes out of the other end.
- The two ends can be in different namespaces. This connects the namespaces.

#### 8. Bridge
- A Linux bridge is a virtual switch. It works at layer 2.
- A bridge moves frames, not packets.
- A bridge learns a table of MAC address to port.
- A bridge floods broadcast frames and frames to unknown MAC addresses to all ports.
- A bridge connects devices in the same layer-2 segment (broadcast domain).
- A bridge does not need an IP address to switch frames.
- You can give the bridge interface an IP address. Then the host can use the bridge as a gateway. Example: Docker gives `docker0` the address `172.17.0.1`.

#### 9. Linux interface
- An interface is an object in the kernel. It represents a network device.
- An interface can be physical, such as an Ethernet card.
- An interface can be virtual, such as `lo`, a veth end or a bridge.

#### 10. Route table
- The route table tells where to send a packet for a given destination.
- Each route has a destination prefix and a next hop.
- The next hop is an IP address (`via`) and an interface (`dev`).
- A connected route has no `via`. Example: `10.0.0.0/24 dev veth0`. The destination is on the same link. The host sends ARP for the destination itself.
- If many routes match, the route table uses the longest prefix match. The most specific route wins.
- The gateway IP never goes into the packet. The host uses the gateway IP only to find the gateway MAC with ARP.

#### 11. Default route
- The default route is `0.0.0.0/0`.
- `/0` is the shortest prefix. It matches all destinations.
- Thus, the default route is used only when no other route matches.
- If no route matches and there is no default route, the result is "Network is unreachable". The host sends no packet.

#### 12. iptables
- iptables filters and changes packets.
- iptables has tables:
  - `filter`: accept or drop packets.
  - `nat`: change addresses and ports.
  - `mangle`: change other packet fields.
  - `raw`: settings before connection tracking.
- Each table puts its chains on some of the five hooks:
  - PREROUTING: the packet just came in. The routing decision did not occur yet.
  - INPUT: the destination of the packet is this device.
  - FORWARD: the packet goes through this device to another device.
  - OUTPUT: this device made the packet.
  - POSTROUTING: the packet is about to go out.

#### 13. NAT types
- SNAT:
  - SNAT changes the source address.
  - Use it when traffic goes out of a private network.
  - It is in the POSTROUTING chain.
  - SNAT writes a fixed IP that you give it.
- MASQUERADE:
  - MASQUERADE is a type of SNAT.
  - It uses the current IP of the outgoing interface.
  - Use it when the outgoing IP can change, for example with DHCP.
- DNAT:
  - DNAT changes the destination address and port.
  - Use it for port forwarding.
  - It is in the PREROUTING chain.

#### 14. How a packet moves from end to end
1. The host makes an IP packet with the source IP and the destination IP.
2. The host looks in its route table for the destination. It uses the longest prefix match.
3. If the route has a `via`, the next hop is the gateway. If not, the next hop is the destination itself.
4. The host looks in its ARP cache for the MAC of the next hop.
5. If the MAC is not in the cache, the host sends an ARP request.
6. The host puts the IP packet in an Ethernet frame. The frame has the host MAC as source and the next-hop MAC as destination.
7. The host sends the frame on the link.
8. The router receives the frame. It removes the old Ethernet header.
9. The router decreases the TTL by 1. The IP addresses do not change (except with NAT).
10. The router forwards only when `ip_forward=1`.
11. The router repeats steps 2 to 7 with its own route table.
12. The reply must also have a route back to the source.
