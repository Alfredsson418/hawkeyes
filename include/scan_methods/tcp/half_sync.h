#pragma once

#include <netinet/tcp.h>
#include <string.h>

#include "../../other/checksum.h"
#include "../../other/headers/ip.h"
#include "../../other/headers/tcp.h"
#include "../scan_structs.h"
#include "../socket_setup.h"

int SYN_scan(scan_arg_t arg, scan_result_t *result);
