#pragma once

#include <stdlib.h>
#include <pcap.h>
#include <unistd.h> // sleep
#include <pthread.h>
#include <string.h> // memcpy

#include "../constans.h"
#include "../ui/print_macro.h"
#include "../other/network_interface.h"



typedef struct {
    struct pcap_pkthdr * packet_header;
    u_char * packet_payload;
} net_packet;

typedef struct {
    int timeout;
    char filter[80];
    char * interface;
    bool setup_complete;
} next_best_args;


/*
    Gives the first package from a given filter.

    Parameters:
        network_interface: The network interface to scan from.
        filter: A more specific filter for capture (Can be NULL).
        timeout: Timeout in sec.

    Return:
        The packet with the packet_header.
*/
net_packet * next_best_packet(next_best_args * in_args);
