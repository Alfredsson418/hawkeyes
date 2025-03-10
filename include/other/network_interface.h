#pragma once

#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include "../constans.h"
#include "../ui/print_macro.h"
#include "permissions.h"
#include "ping.h"

typedef struct {
	char					name[INTERFACE_LEN];
	struct sockaddr_storage s_addr;
	struct sockaddr_storage subnet_mask;
} interface_info;

/*
	Gives the first given network interface.

	Parameters:
		None

	Return:
		Returns first device from pcap_findalldevs, else NULL.
*/
int get_first_network_interface(interface_info *interface);

/*
	Parameters:
		ip: The IP to ping
		interface: pointer to address to copy too
	Return:
	Returns -1 if error, else 0
*/
int guess_interface(struct sockaddr_storage ip_addr, interface_info *interface);

/*
	Parameters:
		interface: The iterface to send packages from
		OBS! Specifiy what network family to use
*/
int verify_interface(interface_info *interface);
