#include "../../../include/scan_methods/tcp/half_sync.h"

// https://nmap.org/book/synscan.html
//
// https://linux-kernel-labs.github.io/refs/heads/master/labs/networking.html

int SYN_scan(scan_arg_t arg, scan_result_t *result) {
    unsigned int packet_len = sizeof(struct iphdr) + sizeof(struct tcphdr);
    char         packet[packet_len];

    struct iphdr  *ip_hdr  = (struct iphdr *)packet;
    struct tcphdr *tcp_hdr = (struct tcphdr *)(packet + sizeof(struct iphdr));

    int sock = socket_init(SOCK_RAW, IPPROTO_RAW, arg);

    if (arg.addr->ss_family == AF_INET) {
        ((struct sockaddr_in *)arg.addr)->sin_port = htons(arg.port);
    } else if (arg.addr->ss_family == AF_INET6) {
        ((struct sockaddr_in6 *)arg.addr)->sin6_port = htons(arg.port);
    }

    memset(tcp_hdr, 0, sizeof(struct tcphdr));

    pseudo_hdr.source_address =
        ((struct sockaddr_in *)&arg.interface->s_addr)->sin_addr.s_addr;
    pseudo_hdr.dest_address = ((struct sockaddr_in *)arg.addr)->sin_addr.s_addr;
    pseudo_hdr.placeholder  = 0;
    pseudo_hdr.protocol     = IPPROTO_TCP;
    pseudo_hdr.tcp_length   = htons(sizeof(struct tcphdr));

    sendto(sock, packet, packet_len, 0, (struct sockaddr *)arg.addr,
           get_addr_len(arg.addr));

    return 0;
}
