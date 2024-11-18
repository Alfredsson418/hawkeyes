#pragma once

#include "../netspectre.h"
#include "capture_arguments.h"
#include "../other/network_device.h"
#include "packet_info.h"
#include "../other/replace.h"
#include "../other/hexdump.h"

/*
    Binary to Mac.

    Parameters:
        mac_bin: Mac address in representet in binary.
        mac: The function writes the MAC address to this varible.

    Return:
        Void
*/
void bin_to_mac(uint8_t mac_bin[6], char mac[18]);

/*
    The capture main function.

    Parameters:
        argc: Program start argument length.
        argv: The array of start arguments.

    Return:
        If run successfully.
*/
int capture(int argc, char *argv[]);
