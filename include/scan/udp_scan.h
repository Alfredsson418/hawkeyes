#pragma once

#include <time.h>

#include "../netspectre.h"
#include "../capture/next_best_packet.h"
#include "../other/convert_time.h"

/*
    Parameters:
        ip: The target to scan.
        port: The port to scan.
        interface: The network interface to scan from.
        timeout: Timeout in sec, will affect scanning.
    Return:
        If run successfully
*/
int udp_scan(scan_arg_t arg, scan_result_t * result);
