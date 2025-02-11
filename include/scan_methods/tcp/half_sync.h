#pragma once

#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <string.h>
#include <sys/socket.h>

#include "../../other/checksum.h"
#include "../../other/convert_time.h"
#include "../../other/headers/ip.h"
#include "../../other/headers/tcp.h"
#include "../../other/next_best_packet.h"
#include "../scan_structs.h"
#include "../socket_setup.h"

int SYN_scan(scan_arg_t arg, scan_result_t *result);
