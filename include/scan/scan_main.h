#pragma once

#include "../netspectre.h"
#include "terminal_scan_arguments.h"
#include "multithread_scanning.h"
#include "tcp_scan.h"
#include "udp_scan.h"

/*
    Parameters:
        argc: Program start argument length.
        argv: The array of start arguments.
    Return:
        If run
*/
int scan(int argc, char *argv[]);
