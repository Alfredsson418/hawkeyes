Hawkeyes is created and made for UNIX based systems and is using UNIX specific libraries,
therefore  other OS is not directly compatible. If you want to contribute and make a working Windows (or other)
build, be aware that it should not break anything for the UNIX build (e.g if UNIX and Windows should use the same source code).
No Windows build is planned.

OBS: Some libraries used in this project only work for UNIX based systems (argp) or work differently from different OS (libpcap),
so keep that in mind when compiling and running this program.


# Non standard libraries used
* libpcap-dev (Used to capture traffic for UDP)
* argp (Used for parsing arguments from terminal)
* libcap-dev (Used to check permissions)
