#pragma once

#include <arpa/inet.h>
#include <ifaddrs.h>
#include <string.h>

#include "../constans.h"
#include "../ui/print_macro.h"
#include "ping.h"

/*
    Gives the first given network interface.

    Parameters:
        None

    Return:
        Returns first device from pcap_findalldevs, else NULL.
*/
int get_first_network_interface(char (*interface)[INTERFACE_LEN]);

/*
    Parameters:
        ip: The IP to ping
        interface: pointer to address to copy too
    Return:
    Returns -1 if error, else 0
*/
int guess_interface(struct sockaddr_storage ip_addr,
                    char (*interface)[INTERFACE_LEN]);

int verify_interface(char interface[INTERFACE_LEN]);
