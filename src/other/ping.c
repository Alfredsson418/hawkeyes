#include "../../include/other/ping.h"
#include <unistd.h>

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

char * guess_ping(struct in_addr ip_addr) {

    // Get network interfaces
    struct ifaddrs * network_devices;
    if (getifaddrs(&network_devices) < 0) {
        ERR_PRINT("%s\n", "ifaddrs");
        return NULL;
    }

    // Start looping though interfaces to ping
    for (; network_devices != NULL; network_devices = network_devices->ifa_next) {
        // If the network interface has an IPv4 adress

        if (network_devices->ifa_addr != NULL && network_devices->ifa_addr->sa_family == AF_INET) {
            // VERBOSE_MESSAGE("Trying on %s\n", network_devices->ifa_name);

            int respone;
            VERBOSE_MESSAGE("PING REQUEST: Trying on '%s'\n", network_devices->ifa_name);
            for (int i = 1; i <= 2; i++) {
                VERBOSE_MESSAGE("PING REQUEST: Attempt %d", i);
                respone = ping(ip_addr, network_devices->ifa_name);
                if (respone > 0) {
                   VERBOSE_MESSAGE(" %s\n", "SUCCESSFULL");
                    break;
                }
                VERBOSE_MESSAGE(" %s\n", "FAILED");

            }
            if (respone == true) {
                char * device = calloc(strlen(network_devices->ifa_name) + 1, sizeof(char));
                strcpy(device, network_devices->ifa_name);
                return device;
            }
            // sleep(1);
        }
    }
    VERBOSE_MESSAGE("Target %s cant be reached\n", inet_ntoa(ip_addr));
    return 0;
}


// Sometimes this gets no responce, even though target is up
int ping(struct in_addr ip_addr, const char * interface) {

    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sock < 0) {
        ERR_PRINT("%s\n", "An error occured when truing to open a Raw socket, make sure to run the program as root!");
        return -1;
    }

    // Bind the socket to the specified network interface
    //printf("TEST INTERFACE %s\n", interface);
    if (setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, interface, strlen(interface)) < 0) {
        ERR_PRINT("Failed to bind to device %s: %s\n", interface, strerror(errno));
        close(sock);
        return -1;
    }


    // Set a timeout for recvfrom
    struct timeval timeout;
    timeout.tv_sec = 5; // 5 seconds timeout
    timeout.tv_usec = 0;
    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        ERR_PRINT("%s\n", "Failed to set a timeout to socket");
        close(sock);
        return -1;
    }

    // Setting up ICMP echo package
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(0);
    addr.sin_addr = ip_addr;
    //memcpy(&addr.sin_addr.s_addr, &ip_addr.s_addr, sizeof(ip_addr.s_addr));
    struct icmphdr icmp_hdr;
    memset(&icmp_hdr, 0, sizeof(icmp_hdr));
    icmp_hdr.type = ICMP_ECHO;
    icmp_hdr.un.echo.id = getpid();
    icmp_hdr.un.echo.sequence = htons(1);
    icmp_hdr.code = 0;
    icmp_hdr.checksum = checksum(&icmp_hdr, sizeof(icmp_hdr));


    // Creating buffer for recvfrom
    char buffer[1024];
    struct sockaddr_in sender_addr;
    socklen_t sender_addr_len = sizeof(sender_addr);

    // Pinging adress on interface
    ssize_t sent = sendto(sock, &icmp_hdr, sizeof(icmp_hdr), 0, (struct sockaddr *)&addr, sizeof(addr));
    if (sent < 0) {
        ERR_PRINT("%s\n", "An error occured when trying to send a ICMP packet from an raw socket");
        close(sock);
        return -1;
    }

    int tries = 2;

    // This is needed because the first packet could be the package sent
    for (int i = 0; i < tries; i++) {
        // Recovering echo packet
        ssize_t received = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&sender_addr, &sender_addr_len);
        if (received < 0) {
            // perror("recvfrom failed");
            ERR_PRINT("%s\n", "Did not recover any data before timeout");
            close(sock);
            return -1;
        }

        // Process the received packet (e.g., check ICMP header, etc.)
        struct iphdr *ip_hdr = (struct iphdr *)buffer;
        struct icmphdr *recv_icmp_hdr = (struct icmphdr *)(buffer + (ip_hdr->ihl * 4));

        // printf("%d %d \n", recv_icmp_hdr->type, recv_icmp_hdr->un.echo.id);

        if (recv_icmp_hdr->type == ICMP_ECHOREPLY && recv_icmp_hdr->un.echo.id == getpid()) {
            close(sock);
            return 1;
        }
    }

    close(sock);
    return 0;
}
