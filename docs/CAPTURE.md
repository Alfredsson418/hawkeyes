# Usage
`netspec capture [options]`
### For help
`netspec capture --usage`
`netspec capture --help`

# Options

## Format
`--format FORMAT`
Specifies output FORMAT for captured package.
Default `FOMRAT` value:

```
Package header: \n
    Time of capture: {head-time}\n
    Package Length: {head-origin-len}\n
    Captured Length: {head-capture-len}\n
 Layer 2:\n"
    MAC-SRC: {mac-src}\n
    MAC-DST: {mac-dst}\n
    Protocol: {mac-prot}\n"
 Layer 3:\n
    IPv4-SRC: {ipv4-src}\n
    IPv4-DST: {ipv4-dst}\n
    Protocol: {ipv4-prot}({ipv4-prot-num})
```
Valid `FILTER` arguments
* `head-capture-len` Captured size
* `head-origin-len` Total package size
* `head-time` Time when packet was captured (based on The Unix Epoch)
* `mac-src` Source MAC address
* `mac-dst` Destination MAC address
* `mac-prot` Layer 2 Protocol e.g IPv4 and ARP
* `IPv4-src` Source IPv4 address
* `IPv4-dst` Destination IPv4 address
* `l3-prot` Layer 3 Protocol e.g TCP and UDP
* `l3-prot-num` Layer 3 Protocol number e.g 6 (TCP)
* `l3-prot-vers` Protocol version, e.g 4 is IPv4
* `ttl` Time to live

## Device
`--device DEVICE` `-d DEVICE`
Specifies the `DEVICE` to capture from.
Default value will be the first device found.
Default `DEVICE` value: `None`
## Hexdump
`--hexdump`
Output hexdump from package.

## Pcap Load
`--pcap-load FILE`
Load already captured packages from `FILE`.
Default value: `None`

# Filter
`--filter FILTER` `-f FILTER`
Will filter captured traffic after `FILTER`.
Default `FILTER` value: `None`

## Capture Amount
`--capture-amount AMOUNT`
Capture ONLY the `AMOUNT` specified. If 0 is passed as an value, then the program will capture forever.
Default `AMOUNT`value: `0`
