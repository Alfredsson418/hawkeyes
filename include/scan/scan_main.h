#pragma once

#include "../netspectre.h"
#include "terminal_scan_arguments.h"
#include "multithread_scanning.h"
#include "tcp_scan.h"
#include "udp_scan.h"
#include "../other/service_detection.h"
#include "../other/transfer_layers.h"
#include "port_scan_loopup.h"

#define RESULT_PORT_LEN 10 //65535 + /tcp or /udp or other
#define RESULT_SERVICE_LEN PORT_SERVICE_LEN
#define RESULT_METHOD_LEN 20
#define RESULT_TIME_LEN 6
#define RESULT_STATE_LEN 8


/*
    Parameters:
        argc: Program start argument length.
        argv: The array of start arguments.
    Return:
        If run
*/
int scan(int argc, char *argv[]);
