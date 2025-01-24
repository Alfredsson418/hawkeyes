#include "../../../include/scan_methods/tcp/half_sync.h"
#include <arpa/inet.h>

// https://nmap.org/book/synscan.html
//
// https://linux-kernel-labs.github.io/refs/heads/master/labs/networking.html

int SYN_scan(scan_arg_t arg, scan_result_t *result) {
    unsigned int   packet_len = sizeof(struct iphdr) + sizeof(struct tcphdr);
    char           packet[packet_len];
    struct iphdr  *ip_hdr  = (struct iphdr *)packet;
    struct tcphdr *tcp_hdr = (struct tcphdr *)(packet + sizeof(struct iphdr));
    tcp_pseudo_header pseudo_hdr;
    memset(&pseudo_hdr, 0, sizeof(pseudo_hdr));
    struct sockaddr_in *dst = (struct sockaddr_in *)arg.addr;
    struct sockaddr_in *src = (struct sockaddr_in *)&arg.interface->s_addr;

    int sock = socket_init(SOCK_RAW, IPPROTO_RAW, arg);

    if (arg.addr->ss_family == AF_INET) {
        ((struct sockaddr_in *)arg.addr)->sin_port = htons(arg.port);
    } else if (arg.addr->ss_family == AF_INET6) {
        ((struct sockaddr_in6 *)arg.addr)->sin6_port = htons(arg.port);
    }

    construct_ip_hdr(ip_hdr, (struct sockaddr_in *)&arg.interface->s_addr,
                     (struct sockaddr_in *)arg.addr, 1234,
                     sizeof(struct tcphdr));

    memset(tcp_hdr, 0, sizeof(struct tcphdr));

    tcp_hdr->source  = htons(54302);
    tcp_hdr->dest    = htons(arg.port);
    tcp_hdr->seq     = htonl(1105024978);
    tcp_hdr->ack_seq = 0;
    tcp_hdr->res1    = 0;
    tcp_hdr->res2    = 0;
    tcp_hdr->doff    = sizeof(struct tcphdr) / 4;
    tcp_hdr->window  = htons(65495);

    tcp_hdr->fin   = 0;
    tcp_hdr->syn   = 1;
    tcp_hdr->rst   = 0;
    tcp_hdr->psh   = 0;
    tcp_hdr->ack   = 0;
    tcp_hdr->urg   = 0;
    tcp_hdr->check = 0;

    pseudo_hdr.source_address = src->sin_addr.s_addr;
    pseudo_hdr.dest_address   = dst->sin_addr.s_addr;
    pseudo_hdr.placeholder    = 0;
    pseudo_hdr.protocol       = IPPROTO_TCP;
    pseudo_hdr.tcp_length     = htons(sizeof(struct tcphdr));

    unsigned int pseudo_size =
        sizeof(tcp_pseudo_header) + sizeof(struct tcphdr);

    char pseudogram[pseudo_size];

    memcpy(pseudogram, &pseudo_hdr, sizeof(tcp_pseudo_header));
    memcpy(pseudogram + sizeof(tcp_pseudo_header), tcp_hdr,
           sizeof(struct tcphdr));

    tcp_hdr->check = checksum(pseudogram, pseudo_size);

    sendto(sock, packet, packet_len, 0, (struct sockaddr *)arg.addr,
           get_addr_len(arg.addr));

    return 0;
}
