#pragma once

#include <time.h>

#include "../netspectre.h"
#include "../other/convert_time.h"

/*
    Parameters:
        ip: The target to scan.
        port: The port to scan.
        timeout: Time before connection fails
    Return:
        If run successfully
*/
int tcp_scan(scan_arg_t arg, scan_result_t  * result);
