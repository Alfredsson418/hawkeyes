#pragma once

#include <sys/socket.h>
#include <unistd.h>
#include <string.h> // memset

#include "../ui/print_macro.h"
#include "../scan_structs.h"
#include "../other/convert_time.h"

int tcp_scan_init(scan_arg_t arg);

/*
    Parameters:
        ip: The target to scan.
        port: The port to scan.
        timeout: Time before connection fails
    Return:
        If run successfully
*/
int tcp_scan(int sock, scan_arg_t arg, scan_result_t * result);

void tcp_scan_free(int sock);
