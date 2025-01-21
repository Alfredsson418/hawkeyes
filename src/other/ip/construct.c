#include "../../../include/other/ip/construct.h"
#include <netinet/in.h>

int construct_ipv4(struct iphdr *hdr, struct sockaddr_storage *addr) {
    struct sockaddr_in *v4 = (struct sockaddr_in *)addr;

    // IPv4
    hdr->version = 4;

    // IP header size (5*4=20 bytes) Standard without options are 20 bytes
    hdr->ihl = 5; // HELEN, Header length

    // Type of service
    hdr->tos = 0;

    // sizeof the packet and payload, should be done outside of this function
    // hdr->tot_len =

    // id of the packet, should also be done outside of this function
    // hdr->id =

    // Flags and Fragmentation offset, used when
    hdr->frag_off = 0;

    // Amount of routers that the packet can hop before being discarded
    hdr->ttl = 64;

    // TCP (6), UDP (17)
    hdr->protocol = IPPROTO_TCP;

    // Checksum of the IP header
    // Should be calculated
    hdr->check = 0;

    return 0;
}

int construct_ipv6(struct iphdr *hdr) { return 0; }
