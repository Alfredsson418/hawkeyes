#pragma once

#include "../netspectre.h"


/*
    Parameters:
        ip: The IP to ping
        interface: pointer to address to copy too
    Return:
    Returns -1 if error, else 0
*/
int guess_ping(struct in_addr ip_addr, char (*interface)[INTERFACE_LEN]);

/*
    Parameters:
        ip: The IP to ping
        interface: what network interface to ping from
    Return:
        true if reach, false if not, -1 for errors
*/
int ping(struct in_addr ip_addr, const char * interface);
