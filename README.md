# Hawkeyes
Hawkeyes is a lightweight and efficient port scanner designed for network diagnostics. Hawkeyes supports both IPv4 and IPv6 and utilizes multithreading for scanning multiple ports at the same time.

## [`Usage`](docs/Help.md)
`hawk [options]`
## Example
`hawk -t 127.0.0.53 -p 53-55,60 -i lo -m conn`

`sudo hawk -t ::1 -p 5353 -i lo -m icmp`

## How to compile
1. Download necessary libraries(read [`requirements`](docs/Requirements.md))
2. Run `make` in the project root folder
3. Done !

# Contribution
TBA

# Questions
- This project is not named after Hawkeye(Marvel)
