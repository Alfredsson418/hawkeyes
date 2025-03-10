#pragma once

#include <string.h> // memset
#include <sys/socket.h>
#include <unistd.h>

#include "../../other/convert_time.h"
#include "../../ui/print_macro.h"
#include "../scan_structs.h"
#include "../socket_setup.h"

/*
	Parameters:
		ip: The target to scan.
		port: The port to scan.
		timeout: Time before connection fails
	Return:
		If run successfully
*/
int connect_scan(scan_arg_t arg, scan_result_t *result);
