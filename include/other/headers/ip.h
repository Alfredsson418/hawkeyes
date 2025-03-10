#pragma once

#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <string.h>

#include "../checksum.h"

int ip_hdr_setup(struct iphdr *hdr, struct sockaddr_in *s_addr,
				 struct sockaddr_in *d_addr, unsigned int id,
				 unsigned int payload_len);

int ip6_hdr_setup(struct ip6_hdr *hdr, struct sockaddr_in6 *s_addr,
				  struct sockaddr_in6 *d_addr, unsigned int id,
				  unsigned int payload_len);
