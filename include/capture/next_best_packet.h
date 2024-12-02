#pragma once

#include "../netspectre.h"
#include "../other/network_device.h"

/*
    Gives the first package from a given filter.

    Parameters:
        network_interface: The network interface to scan from.
        filter: A more specific filter for capture (Can be NULL).
        timeout: Timeout in sec.

    Return:
        The packet with the packet_header.
*/
net_packet * next_best_packet(const char * network_interface,char * filter, int timeout, bool * setup_complete);
