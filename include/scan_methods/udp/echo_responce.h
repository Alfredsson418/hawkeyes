#pragma once

#include <arpa/inet.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../../other/convert_time.h"
#include "../../other/next_best_packet.h"
#include "../../ui/print_macro.h"
#include "../scan_structs.h"
#include "../socket_setup.h"

int icmp_responce_scan(scan_arg_t arg, scan_result_t *result);
