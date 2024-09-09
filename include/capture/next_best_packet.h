#ifndef NEXT_BEST_PACKET_H
#define NEXT_BEST_PACKET_H

#include "../netspectre.h"
#include "../other/network_device.h"

/*
    Gives the first package from a given filter.

    Parameters:
        network_device: The network device to scan from.
        filter: A more specific filter for capture (Can be NULL).
        timeout: Timeout in sec.

    Return:
        The packet with the packet_header.
*/
net_packet * next_best_packet(char * network_device ,char * filter, int timeout);


#endif