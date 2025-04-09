Hawkeyes is created and made for linux based systems and is using linux specific libraries,
therefore  other OS is not directly compatible. If you want to contribute and make a working Windows (or other)
build, be aware that it should not break anything for the linux build (e.g if linux and Windows should use the same source code).
No Windows build is planned.

OBS: Some libraries used in this project only work for linux based systems (argp) or work differently from different OS (libpcap),
so keep that in mind when compiling and running this program.

# Software requirements
* libpcap-dev on Debian, libpcap-devel on Fedora
* argp (Used for parsing arguments from terminal)
* libcjson-dev on Debian, libpcap-devel on Fedora (https://github.com/DaveGamble/cJSON)

# Development requirements
* jq (Commandline json parser)
* C compiler (like gcc)
* Makefile
