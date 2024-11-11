# Usage
`netspec scan [options]`
### For help
`netspec scan --usage`
`netspec scan --help`

# Options
`--target TARGET` `-t TARGET` Specify the target (only ipv4 for now).

`--protocol METHOD` `-m METHOD` Choose what scanning method to use.
Default: `tcp`
Other options: `udp` & `half-sync`

`--device DEVICE` Specify what network device to ping from.
If not given any, the program will guess the device when trying to ping the target.
If the `no-ping` option is true, the program will use the first given network device found in ifadders (not "lo").

`--port PORT(S)` `-p PORT(S)` Give the program a set number or a range of ports. Default ports are `1-1000`

`--timeout TIME(sec)` `-w TIME(sec)` This is the time before the connection with each port scan will refuse. Defaults to `10` seconds.

`--no-ping` This flag will not ping the target before a scan. Defaults to `false`

`--no-threading` This flag determines if the program should use threading or not. Defaults to `false`

`--threading-workers WORKERS` `-n WORKERS` The amount of threads that should scan the target. Defaults to `3`
