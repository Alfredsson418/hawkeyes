#include "../../../include/scan_methods/tcp/half_sync.h"
#include <netinet/in.h>
#include <netinet/ip6.h>
#include <sys/socket.h>

// https://nmap.org/book/synscan.html
//
// https://linux-kernel-labs.github.io/refs/heads/master/labs/networking.html

int SYN_scan(scan_arg_t arg, scan_result_t *result) {
    unsigned int ip_hdr_len =
        arg.addr->ss_family == AF_INET ? sizeof(struct iphdr) : 0;
    unsigned int   packet_size = sizeof(struct tcphdr) + ip_hdr_len;
    char           packet[packet_size];
    char           recv_packet[2048];
    struct tcphdr *tcp_hdr = (struct tcphdr *)(packet + ip_hdr_len);
    void          *ip_hdr  = packet;

    struct sockaddr_storage client_addr;
    socklen_t               addr_len =
        AF_INET ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6);

    int sock = socket_init(SOCK_RAW, IPPROTO_TCP, arg);

    if (arg.addr->ss_family == AF_INET) {
        // Use this instead of IPPROTO_RAW, because with IPPROTO_RAW, we cant
        // receive any data
        // https://stackoverflow.com/questions/40795772/cant-receive-packets-to-raw-socket
        // ONLY FOR IPV4, IPv6 recv wont include IP header
        int one = 1;
        if (setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &one, sizeof(one)) < 0) {
            perror("setsockopt IP_HDRINCL failed");
            exit(EXIT_FAILURE);
        }

        ((struct sockaddr_in *)arg.addr)->sin_port = htons(arg.port);

        ip_hdr_setup((struct iphdr *)ip_hdr,
                     (struct sockaddr_in *)&arg.interface->s_addr,
                     (struct sockaddr_in *)arg.addr, 100000,
                     sizeof(struct tcphdr));

    } else {
        /*
        ip6_hdr_setup((struct ip6_hdr *)ip_hdr,
                      (struct sockaddr_in6 *)&arg.interface->s_addr,
                      (struct sockaddr_in6 *)arg.addr, 100000,
                      sizeof(struct tcphdr));
                      */
        ((struct sockaddr_in6 *)arg.addr)->sin6_port = htons(arg.port);
    }

    tcp_hdr_setup(tcp_hdr, 54300, arg.port, 1105024977, &arg.interface->s_addr,
                  arg.addr);

    sendto(sock, packet, packet_size, 0, (struct sockaddr *)arg.addr, addr_len);

    // First packet may be the sending package and not the receiving package
    for (int i = 0; i < 2; i++) {
        if (recvfrom(sock, recv_packet, 2048, 0,
                     (struct sockaddr *)&client_addr, &addr_len) < 0) {
            result->state = -1;
            perror("recvfrom");
            // return 0;
        }
        struct tcphdr *recv_tcp_hdr;

        if (arg.addr->ss_family == AF_INET) {
            struct iphdr *ip_hdr = (struct iphdr *)recv_packet;
            recv_tcp_hdr = (struct tcphdr *)(recv_packet + (ip_hdr->ihl * 4));
        } else if (arg.addr->ss_family == AF_INET6) {
            recv_tcp_hdr = (struct tcphdr *)(recv_packet);
        }

        if (recv_tcp_hdr->syn == 1 && recv_tcp_hdr->ack == 1) {
            result->state = 1;
            return 1;
        }
        if (recv_tcp_hdr->rst == 1 && recv_tcp_hdr->ack == 1) {
            result->state = 0;
        }
    }

    return 0;
}
