#pragma once

#include <arpa/inet.h>
#include <stdbool.h>
#include <sys/socket.h>

#include "../constans.h"
#include "../other/network_interface.h"
#include "../other/transfer_layers.h"

// #include "constans.h"

// Used to store scanning result from a scan
typedef struct {
	int	   state;
	double scannig_time;
} scan_result_t;

// Used as a universal argument for every scanning method
typedef struct {
	struct sockaddr_storage *addr;
	interface_info			*interface;
	unsigned int			 timeout;
	unsigned short			 port;
} scan_arg_t;

// Used to store information about a scanning functions
// that can be used to set it up
typedef struct {
	char *name;
	bool  needs_root;
	// This is used in case of a raw socket
	transfer_protocol_t transfer_protocol;
	int (*scan_func)(scan_arg_t arg, scan_result_t *result);
} scan_func_t;
