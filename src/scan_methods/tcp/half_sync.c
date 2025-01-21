#include "../../../include/scan_methods/tcp/half_sync.h"
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/tcp.h>

// https://nmap.org/book/synscan.html
//
// https://linux-kernel-labs.github.io/refs/heads/master/labs/networking.html

int SYN_scan(scan_arg_t arg, scan_result_t *result) {
    struct tcphdr tcp_hdr;

    int sock = socket_init(SOCK_RAW, IPPROTO_TCP, arg);

    if (arg.addr->ss_family == AF_INET) {
        ((struct sockaddr_in *)arg.addr)->sin_port = htons(arg.port);
    } else if (arg.addr->ss_family == AF_INET6) {
        ((struct sockaddr_in6 *)arg.addr)->sin6_port = htons(arg.port);
    }

    memset(&tcp_hdr, 0, sizeof(struct tcphdr));

    tcp_hdr.source  = htons(5432);
    tcp_hdr.dest    = htons(arg.port);
    tcp_hdr.seq     = 0;
    tcp_hdr.ack_seq = 0;
    tcp_hdr.res1    = 0;
    tcp_hdr.doff    = 5;

    tcp_hdr.fin   = 0;
    tcp_hdr.syn   = 1;
    tcp_hdr.rst   = 0;
    tcp_hdr.psh   = 0;
    tcp_hdr.ack   = 0;
    tcp_hdr.urg   = 0;
    tcp_hdr.check = 0;

    sendto(sock, 0, 0, 0, (struct sockaddr *)&arg.addr, get_addr_len(arg.addr));
    perror("sock");

    return 0;
}
