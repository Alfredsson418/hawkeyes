#pragma once

#include <ifaddrs.h>
#include <string.h>
#include <arpa/inet.h>

#include "../constans.h"
#include "../ui/print_macro.h"
#include "ping.h"

/*
    Gives the first given network device.

    Parameters:
        None

    Return:
        Returns first device from pcap_findalldevs, else NULL.
*/
int get_first_network_dev(char (*interface)[INTERFACE_LEN]);




/*
    Parameters:
        ip: The IP to ping
        interface: pointer to address to copy too
    Return:
    Returns -1 if error, else 0
*/
int guess_interface(struct in_addr ip_addr, char (*interface)[INTERFACE_LEN]);

int verify_interface(char interface[INTERFACE_LEN]);
