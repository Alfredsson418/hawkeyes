#include "../../include/other/ping.h"

unsigned short checksum(void *b, int len) {
    unsigned short *buf = b;
    unsigned int sum = 0;
    unsigned short result;

    for (sum = 0; len > 1; len -= 2)
        sum += *buf++;
    if (len == 1)
        sum += *(unsigned char *)buf;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}


// Sometimes this gets no responce, even though target is up
int ping(struct in_addr ip_addr, const char interface[INTERFACE_LEN]) {
    int echo_ID = 8765;
    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    int tries = 2;


    struct sockaddr_in addr;
    struct icmphdr icmp_hdr;

    char buffer[1024];
    struct sockaddr_in sender_addr;
    socklen_t sender_addr_len = sizeof(sender_addr);

    // Set a timeout for recvfrom
    struct timeval timeout;
    timeout.tv_sec = 5; // 5 seconds timeout
    timeout.tv_usec = 0;

    if (sock < 0) {
        ERR_PRINT("Falied opening UDP socket for ping\n");
        return -1;
    }

    // Bind the socket to the specified network interface
    if (setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, interface, INTERFACE_LEN) < 0) {
        ERR_PRINT("Failed to bind to device %s: %s\n", interface, strerror(errno));
        close(sock);
        return -1;
    }

    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        ERR_PRINT("Failed to set a timeout to socket\n");
        close(sock);
        return -1;
    }

    // Setting up ICMP echo package

    addr.sin_family = AF_INET;
    addr.sin_port = htons(0);
    addr.sin_addr = ip_addr;
    // memcpy(&addr.sin_addr.s_addr, &ip_addr.s_addr, sizeof(ip_addr.s_addr));

    memset(&icmp_hdr, 0, sizeof(icmp_hdr));
    icmp_hdr.type = ICMP_ECHO;
    icmp_hdr.un.echo.id = echo_ID;
    icmp_hdr.un.echo.sequence = htons(1);
    icmp_hdr.code = 0;
    icmp_hdr.checksum = checksum(&icmp_hdr, sizeof(icmp_hdr));


    // Creating buffer for recvfrom


    // Pinging adress on interface
    ssize_t sent = sendto(sock, &icmp_hdr, sizeof(icmp_hdr), 0, (struct sockaddr *)&addr, sizeof(addr));
    if (sent < 0) {
        ERR_PRINT("Failed to send ICMP/ping packet\n");
        close(sock);
        return -1;
    }

    // This is needed because the first packet could be the package sent
    for (int i = 0; i < tries; i++) {
        // Recovering echo packet
        ssize_t received = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&sender_addr, &sender_addr_len);
        if (received < 0) {
            // ERR_PRINT("Failed to recover any ICMP/ping packages\n");
            close(sock);
            return -1;
        }

        // Process the received packet (e.g., check ICMP header, etc.)
        struct iphdr *ip_hdr = (struct iphdr *)buffer;
        struct icmphdr *recv_icmp_hdr = (struct icmphdr *)(buffer + (ip_hdr->ihl * 4));
        // printf("\n%d, %d, %d \n", recv_icmp_hdr->type, recv_icmp_hdr->un.echo.id, echo_ID);
        if (recv_icmp_hdr->type == ICMP_ECHOREPLY && recv_icmp_hdr->un.echo.id == echo_ID) {
            close(sock);
            return 1;
        }
    }

    close(sock);
    return 0;
}
