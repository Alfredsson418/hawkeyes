#pragma once

#include "../netspectre.h"

/*
    Parameters:
        ip: The target to scan.
        port: The port to scan.
        timeout: Time before connection fails
    Return:
        If run successfully
*/
int tcp_scan(scan_function_arguments arg);
