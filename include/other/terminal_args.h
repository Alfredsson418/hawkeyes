#pragma once

#include <argp.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include "../scan_methods/scan_structs.h"
#include "../scan_methods/tcp/connect.h"
#include "../scan_methods/tcp/half_sync.h"
#include "../scan_methods/udp/echo_responce.h"
#include "../ui/print_macro.h"
#include "ip/convert.h"
#include "ip/domain.h"

extern const struct argp_option terminal_options[];

/*
	0x11 UDP
	0x06 TCP
*/

struct terminal_args {
	char interface[INTERFACE_LEN]; // Risk for overflow or overwriting \0?
	char ports_format[PORTS_FORMAT_LEN];
	struct sockaddr_storage address;
	unsigned int			timeout;
	bool					no_ping;
	unsigned int			thread_workers;
	scan_func_t				scan_info;
	bool					force_parse_service_file;
};

error_t terminal_parse_opt(int key, char *arg, struct argp_state *state);

extern struct argp terminal_argp;
