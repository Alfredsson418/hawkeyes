#pragma once

#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "../../ui/print_macro.h"
#include "../../other/next_best_packet.h"
#include "../../scan_structs.h"


int udp_echo_responce(scan_arg_t arg, scan_result_t * result);
