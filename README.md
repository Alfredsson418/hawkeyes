# NetSpectre
NetSpectre is a swiss army knife of network tools

## Usage
`netspec [tool] [options]`
### Example
`netspec capture -f tcp -x`

`netspec scan -t 127.0.0.1 -i lo -p 53`

## Docs
* [`Capture`](docs/CAPTURE.md) - Capture and show incomming/outgoing traffic
* [`Scan`](docs/SCAN.md) - Scan for open ports on a specific target

## How to compile
1. Download necessary libraries
2. Run `make` in the project root folder
3. Done!

## Libraries used
* libpcap
* argp

# Contribution
TBA
