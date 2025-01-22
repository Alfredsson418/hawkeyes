#include "../../../include/other/ip/construct.h"

int construct_ipv4(struct iphdr *hdr, struct sockaddr_in *s_addr,
                   struct sockaddr_in *d_addr, unsigned int id,
                   unsigned int payload_len) {
    // IPv4
    hdr->version = 4;

    // IP header size (5*4=20 bytes) Standard without options are 20 bytes
    hdr->ihl = 5; // HELEN, Header length

    // Type of service
    hdr->tos = 0;

    // sizeof the packet and payload in network byte order
    hdr->tot_len = htons(payload_len + hdr->ihl * 4);

    // id of the packet, should also be done outside of this function
    hdr->id = htons(id);

    // Flags and Fragmentation offset, used when
    hdr->frag_off = 0;

    // Amount of routers that the packet can hop before being discarded
    hdr->ttl = 64;

    // TCP (6), UDP (17)
    hdr->protocol = IPPROTO_TCP;

    // Source Address (should be the ip of the interface but could also be
    // spoofed)
    hdr->saddr = s_addr->sin_addr.s_addr;

    // Destination Address
    hdr->daddr = d_addr->sin_addr.s_addr;

    // Checksum of the IP header
    hdr->check = 0;
    hdr->check = checksum(hdr, hdr->tot_len * 4);

    return 0;
}

int construct_ipv6(struct iphdr *hdr) { return 0; }
