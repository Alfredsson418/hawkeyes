#pragma once

#include <sys/socket.h>
#include <unistd.h>

#include "../constans.h"
#include "../ui/print_macro.h"
#include "scan_structs.h"

int socket_init(int protocol, scan_arg_t func_arg);

void socket_close(int socket);

unsigned int get_addr_len(struct sockaddr_storage *addr);
