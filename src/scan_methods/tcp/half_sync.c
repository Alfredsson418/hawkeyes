#include "../../../include/scan_methods/tcp/half_sync.h"

// https://nmap.org/book/synscan.html
//
// https://linux-kernel-labs.github.io/refs/heads/master/labs/networking.html

int SYN_scan(scan_arg_t arg, scan_result_t *result) {
    unsigned int ip_hdr_len = arg.addr->ss_family == AF_INET
                                  ? sizeof(struct iphdr)
                                  : sizeof(struct ip6_hdr);
    printf("%d\n", ip_hdr_len);
    unsigned int   packet_size = sizeof(struct tcphdr) + ip_hdr_len;
    char           packet[packet_size];
    void          *ip_hdr  = packet;
    struct tcphdr *tcp_hdr = (struct tcphdr *)(packet + ip_hdr_len);

    int sock = socket_init(SOCK_RAW, IPPROTO_RAW, arg);

    if (arg.addr->ss_family == AF_INET) {
        ((struct sockaddr_in *)arg.addr)->sin_port = htons(arg.port);
        ip_hdr                                     = (struct iphdr *)packet;
        ip_hdr_setup((struct iphdr *)ip_hdr,
                     (struct sockaddr_in *)&arg.interface->s_addr,
                     (struct sockaddr_in *)arg.addr, 100000,
                     sizeof(struct tcphdr));
    } else if (arg.addr->ss_family == AF_INET6) {

        ((struct sockaddr_in6 *)arg.addr)->sin6_port = htons(arg.port);
        ip_hdr                                       = (struct ip6_hdr *)packet;
        ip6_hdr_setup((struct ip6_hdr *)ip_hdr,
                      (struct sockaddr_in6 *)&arg.interface->s_addr,
                      (struct sockaddr_in6 *)arg.addr, 100000,
                      sizeof(struct tcphdr));
    }

    tcp_hdr_setup(tcp_hdr, 54302, arg.port, 1105024978, &arg.interface->s_addr,
                  arg.addr);

    sendto(sock, packet, packet_size, 0, (struct sockaddr *)arg.addr,
           get_addr_len(arg.addr));
    perror("sock");
    return 0;
}
