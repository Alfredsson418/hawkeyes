#include "../../../include/scan_methods/tcp/half_sync.h"

// https://nmap.org/book/synscan.html
//
// https://linux-kernel-labs.github.io/refs/heads/master/labs/networking.html

int SYN_scan(scan_arg_t arg, scan_result_t *result) {

    int sock = socket_init(SOCK_RAW, arg);

    struct tcphdr tcp_hdr;

    memset(&tcp_hdr, 0, sizeof(tcp_hdr));

    tcp_hdr.dest  = htons(arg.port);
    tcp_hdr.syn   = 1;
    tcp_hdr.ack   = 0;
    tcp_hdr.check = checksum(&tcp_hdr, sizeof(tcp_hdr));

    sendto(sock, &tcp_hdr, sizeof(tcp_hdr), 0, (struct sockaddr *)&arg.addr,
           get_addr_len(arg.addr));

    return 0;
}
