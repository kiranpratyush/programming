# Next steps (resume file)

Started: 2026-10-14

## Status

| Milestone                                               | State                    |
| ------------------------------------------------------- | ------------------------ | ------ |
| D0 design on paper                                      | Not started. Start here. |
| D1 observe DNS in M5                                    |                          | (Done) |
| D2 authoritative (NSD)                                  |                          |
| D3 recursive (Unbound)                                  |                          |
| D4 reverse DNS                                          |                          |
| D5 delegation                                           |                          |
| D6 real world + TTL/pooling                             |                          |
| Artifact: "How a name becomes an address in my network" |                          |

## Next session: D0

1. Answer the D0 questions in README.md under "My design". Use paper or the file, no computer lab yet.
2. Paste the design to Claude for review.
3. Then D1: run `m5-build.sh`, write your prediction for `getent hosts example.com` in `ns-ha1` **before** you run it.
