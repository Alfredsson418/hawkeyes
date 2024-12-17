#pragma once

#include <unistd.h>
#include <netinet/ip_icmp.h>
#include <netinet/icmp6.h>
#include <net/if.h>
#include <string.h> // memset
#include <netinet/ip6.h>

#include "../ui/print_macro.h"
#include "../constans.h"

/*
    Parameters:
        ip: The IP to ping
        interface: what network interface to ping from
    Return:
        true if reach, false if not, -1 for errors
*/
int ping(struct sockaddr_storage ip_addr, const char interface[INTERFACE_LEN]);
