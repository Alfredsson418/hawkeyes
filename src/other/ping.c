#include "../../include/other/ping.h"
#include <linux/if.h>

unsigned short calculate_icmp_checksum(void *b, int len) {    
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

int ping(struct in_addr ip_addr, const char * interface) {
    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sock < 0) {
        ERR_PRINT("%s\n", "An error occured when truing to open a Raw socket, make sure to run the program as root!");
        return NULL;
    }
    PRINT("%s\n", "Created socket");

    // Bind the socket to the specified network interface
    
    if (setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, interface, strlen(interface)) < 0) {
        ERR_PRINT("Failed to bind to device %s: %s\n", interface, strerror(errno));
        close(sock);
        return NULL;
    }

    PRINT("%s\n", "Bound interface");

        // Set a timeout for recvfrom
    struct timeval timeout;
    timeout.tv_sec = 5; // 5 seconds timeout
    timeout.tv_usec = 0;
    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        ERR_PRINT("%s\n", "Failed to set a timeout to socket");
        close(sock);
        return -1;
    }
    
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(0);
    memcpy(&addr.sin_addr.s_addr, &ip_addr.s_addr, sizeof(ip_addr.s_addr));
    struct icmphdr icmp_hdr;
    icmp_hdr.type = ICMP_ECHO;
    icmp_hdr.un.echo.id = 1234;
    icmp_hdr.un.echo.sequence = 1;
    icmp_hdr.checksum = calculate_icmp_checksum(&icmp_hdr, sizeof(icmp_hdr));

    char buffer[1024];
    struct sockaddr_in sender_addr;
    socklen_t sender_addr_len = sizeof(sender_addr);

    PRINT("%s\n", "Sent data");
    ssize_t sent = sendto(sock, &icmp_hdr, sizeof(icmp_hdr), 0, (struct sockaddr *)&addr, sizeof(addr));
    if (sent < 0) {
        ERR_PRINT("%s\n", "An error occured when trying to send a ICMP packet from an raw socket");
        close(sock);
        return -1;
    }
    

    ssize_t received = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&sender_addr, &sender_addr_len);
    if (received < 0) {
        ERR_PRINT("%s\n", "recvfrom");
        close(sock);
        return -1;
    }
    PRINT("%s\n", "Recived data");

    // Process the received packet (e.g., check ICMP header, etc.)
    struct iphdr *ip_hdr = (struct iphdr *)buffer;
    struct icmphdr *recv_icmp_hdr = (struct icmphdr *)(buffer + (ip_hdr->ihl * 4));

    close(sock);


    PRINT("%s\n", "Socket closed");


    // recv_icmp_hdr->type == ICMP_ECHOREPLY && 
    if (recv_icmp_hdr->un.echo.id == 1234) {
        printf("Received ICMP ECHO REPLY from %s\n", inet_ntoa(sender_addr.sin_addr));
        return 0;
    }
    return -1;
}