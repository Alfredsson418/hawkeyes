# Hawkeyes
Hawkeyes is a lightweight and efficient port scanner designed for network diagnostics written in C.

## Features
* Different scanning methods
* IPv4 and IPv6 support
* Specify a range of ports and/or specific ports (e.g., 50-60, 67, 78)
* Multithreaded scanning
* Scan on specific interfaces
* Service detection

## [`Usage`](docs/Help.md)
`hawk [options]`
## Example
`hawk -t 127.0.0.53 -p 53-55,60 -i lo -m conn`

`sudo hawk -t ::1 -p 5353 -i lo -m icmp`

## How to compile
1. Download necessary libraries(read [`requirements`](docs/Requirements.md))
2. Run `make` in the project root folder
3. Done !

# [`Contribution`](docs/Contribution.md)


# Questions
- This project is not named after Hawkeye from Marvel
