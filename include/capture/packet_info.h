#pragma once

#include "../../include/netspectre.h"
#include "../other/protocol_maps.h"
#include "capture_arguments.h"
#include "capture_main.h"
#include "../other/replace.h"

/*

    Parameters:
        arguments: The program arguments
        packet_header: Metadata about captured packet
        packet: The frame or packet that was captured.
    Return:
        Void
*/
void packet_handler(struct capture_arguments * arguments , const struct pcap_pkthdr *packet_header, const unsigned char *packet);

/*
    Parameters:
        packet_header: The packet_header for the packet that was captured.
        format: The input string to format for output
    Return:
        Void
*/
void packet_header_info(const struct pcap_pkthdr *packet_header, char **format);

/*
    Parameters:
        packet: The packet that was captured.
        format: The input string to format for output
    Return:
        Void
*/
void l2_packet_info(const unsigned char *packet, char **format);
void l3_packet_info(const unsigned char *packet, char **format);
