#pragma once

#include <argp.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>

#include "../scan_structs.h"
#include "../ui/print_macro.h"
#include "convert_address.h"

extern const struct argp_option terminal_options[];

/*
    0x11 UDP
    0x06 TCP
*/

struct terminal_args {
    char interface[INTERFACE_LEN];
    unsigned short * ports;
    unsigned short ports_len;
    char ports_format[PORTS_FORMAT_LEN];
    struct sockaddr_storage address;
    scan_methods_t scan_method;
    unsigned int timeout;
    bool no_ping;
    unsigned int thread_workers;
};

error_t terminal_parse_opt(int key, char *arg, struct argp_state *state);

extern struct argp terminal_argp;
