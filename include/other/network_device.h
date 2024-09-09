#ifndef NETWORK_DEVICE_H
#define NETWORK_DEVICE_H

#include "../netspectre.h"

/*
    Gives the first given network device.

    Parameters:
        None

    Return:
        Returns first device from pcap_findalldevs, else NULL.
*/
char * get_first_network_dev();

/*
    Gives the network device based on IP (127.0.0.1 to "lo")

    Parameters:
        ip: The IPv4 adress to seach for

    Return:
        Returns the given network device
*/
char * get_net_dev_by_ip(char ip[IPV4_ADDR_STR_LEN]);

void free_dev(char * mem);

#endif