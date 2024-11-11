#pragma once

#include "../netspectre.h"
#include "../capture/next_best_packet.h"

/*
    Parameters:
        ip: The target to scan.
        port: The port to scan.
        interface: The network interface to scan from.
        timeout: Timeout in sec, will affect scanning.
    Return:
        If run successfully
*/
int udp_scan(scan_function_arguments arg);
