#pragma once

#include <arpa/inet.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <unistd.h>

#include <fcntl.h>
#include <termios.h>

#include "../other/convert_time.h"
#include "../other/permissions.h"
#include "../other/port_scan_lookup.h"
#include "../ui/print_macro.h"
#include "scan_structs.h"
#include "socket_setup.h"

int multithread_scanning(scan_func_t *func_info, scan_arg_t *func_arg,
                         scan_result_t *func_result, unsigned short *ports,
                         unsigned int port_len, unsigned int workers);
