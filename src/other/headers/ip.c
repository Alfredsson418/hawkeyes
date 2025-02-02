#include "../../../include/other/headers/ip.h"

int ip_hdr_setup(struct iphdr *hdr, struct sockaddr_in *s_addr,
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
    // Could only set to 0 because the OS will calculate on its own
    hdr->check = 0;
    // hdr->check = htons(0x0000); May be overwritten by the os

    return 0;
}

// https://www.geeksforgeeks.org/internet-protocol-version-6-ipv6-header/
int ip6_hdr_setup(struct ip6_hdr *hdr, struct sockaddr_in6 *s_addr,
                  struct sockaddr_in6 *d_addr, unsigned int flow_id,
                  unsigned int payload_len) {
    memset(hdr, 0, sizeof(struct ip6_hdr));

    // hdr->ip6_ctlun.ip6_un2_vfc          = 0x60;
    hdr->ip6_src                        = s_addr->sin6_addr;
    hdr->ip6_dst                        = d_addr->sin6_addr;
    hdr->ip6_ctlun.ip6_un1.ip6_un1_hlim = 64;
    hdr->ip6_ctlun.ip6_un1.ip6_un1_nxt  = IPPROTO_TCP;
    hdr->ip6_ctlun.ip6_un1.ip6_un1_plen = htons(payload_len);

    // This can be used for both version, traffic class and flow label
    hdr->ip6_ctlun.ip6_un1.ip6_un1_flow = 0x60;

    return 0;
}
