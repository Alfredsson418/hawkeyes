#pragma once

#include <net/if.h>
#include <netinet/icmp6.h>
#include <netinet/ip6.h>
#include <netinet/ip_icmp.h>
#include <string.h> // memset
#include <unistd.h>

#include "../constans.h"
#include "../ui/print_macro.h"
#include "checksum.h"

/*
	Parameters:
		ip: The IP to ping
		interface: what network interface to ping from
	Return:
		true if reach, false if not, -1 for errors
*/
int ping(struct sockaddr_storage ip_addr, const char interface[INTERFACE_LEN]);
