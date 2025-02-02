#pragma once

#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "../checksum.h"

// Pseudo header needed for TCP checksum calculation
struct pseudo_header {
    u_int32_t source_address;
    u_int32_t dest_address;
    u_int8_t  placeholder;
    u_int8_t  protocol;
    u_int16_t tcp_length;
};

struct ip6_pseudo_header {
    uint8_t  src_addr[16];
    uint8_t  dst_addr[16];
    uint32_t upper_layer_packet_length;
    uint8_t  zero[3];
    uint8_t  next_header;
};

int tcp_hdr_setup(struct tcphdr *tcp_hdr, unsigned short src_port,
                  unsigned short dst_port, unsigned int seq_id,
                  struct sockaddr_storage *src_addr,
                  struct sockaddr_storage *dst_addr);
