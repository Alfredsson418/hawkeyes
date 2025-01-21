#pragma once

#include <netinet/tcp.h>
#include <string.h>

#include "../../other/checksum.h"
#include "../scan_structs.h"
#include "../socket_setup.h"

int SYN_scan(scan_arg_t arg, scan_result_t *result);
