# Usage
`hawk [options]`
### For help
`hawk --usage`

`hawk --help`

`hawk -?`

# Options
## Target
`--target IP`

`-t IP`

Specify the target (only ipv4 for now).

## Port
`--port PORT(S)`

`-p PORT(S)`

Give the program a set number or a range of ports. Default ports are `1-1000`


## Protocol
`--method METHOD`


`-m METHOD` Choose what scanning method to use.

### Methods
- conn

`conn` or connect() is the regular way to connect to a TCP service though a 3 way handshake.

- icmp (OBS! Requires sudo permissions to run)

`icmp` or ICMP Echo reply is used for UDP port scanning. This method utilized the fact that some firewalls reply with "Port Unreachable" or "Destination Unreachable" for UDP ports that are not open.
This works mostly on older firewalls because modern firewalls will not responde at all for this very reason.


## Interface
`--interface INTERFACE`

`-i INTERFACE`

Specify what network device to ping from.

NOTE: If not defined, the program will choose the first interface from what ifadders returns (not `lo`).
If run with additional privileges, the program will attempt to guess the interface, unless the `--no-ping` flag is set.

## No Ping
`--no-ping`

Do not ping the target before scanning. Defaults to `false`, will be **ignored** if not run with additional privileges.

## Workers
`--threading-workers WORKERS`

`-n WORKERS`

The amount of threads that should scan the target. Defaults to `3`.

## Timeout
`--timeout TIME(sec)`

`-w TIME(sec)`

This is the time before the connection with each port scan will refuse. Defaults to `3` seconds.
