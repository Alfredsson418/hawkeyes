#pragma once

#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "../ui/print_macro.h"
#include "../other/next_best_packet.h"
#include "../scan_structs.h"
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
