#pragma once

#include <netinet/ip.h>
#include <netinet/ip6.h>

#include "../checksum.h"

int construct_ipv4(struct iphdr *hdr, struct sockaddr_in *s_addr,
                   struct sockaddr_in *d_addr, unsigned int id,
                   unsigned int payload_len);
