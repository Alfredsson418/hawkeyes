<h3 align="center" ><img alt="Hawkeyes picure" src="data/images/hawkeyes2.png"></h3>
<h3 align="center">Lightweight and efficient port scanner designed for network diagnostics written in C.</h3>
<br>
<p align="center">
  <img alt="GitHub Repo stars" src="https://img.shields.io/github/stars/Alfredsson418/hawkeyes?style=for-the-badge">
  <img alt="GitHub repo size" src="https://img.shields.io/github/repo-size/Alfredsson418/hawkeyes?style=for-the-badge&color=darkorange">
  <img alt="GitHub top language" src="https://img.shields.io/github/languages/top/Alfredsson418/hawkeyes?style=for-the-badge&color=mediumaquamarine">
  <img alt="Website" src="https://img.shields.io/website?url=https%3A%2F%2Fhawkeyes.dev&style=for-the-badge">

</p>

## Current Features
* Different scanning methods
* IPv4 and IPv6 support
* Specify a range of ports and/or specific ports (e.g., 50-60, 67, 78)
* Multithreaded scanning
* Scan on specific interfaces
* Service detection
## Future Features
* Guess service on port with probes
* More scanning methods

# How to run
## Compile
1. Download necessary libraries(read [`requirements`](docs/Requirements.md))
2. Run `make` in the project root folder
3. Done !

## [`Usage`](docs/Help.md)
`./hawk [options]`
### Example
`./hawk -t 127.0.0.53 -p 53-55,60 -i lo -m conn`

`sudo ./hawk -t ::1 -p 5353 -i lo -m icmp`


# [`Contribution`](docs/Contribution.md)
TBA

# Questions
- This project is not named after Hawkeye from Marvel
