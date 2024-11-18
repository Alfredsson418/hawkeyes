#pragma once

#include "../netspectre.h"

/*
    Parameters:
        packet_protocol: Integer of 16 bits that represent a specific protocol
        layer: An integer that represent what layer we are working on, 2 represent layer 2 (ethertype)
    Return:
        Returns the name of the protocol
*/
char * determine_packet_protocol(uint16_t packet_protocol, int layer);
