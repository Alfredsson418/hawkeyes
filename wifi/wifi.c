#include <stdio.h>
#include <stdlib.h>
#include <iwlib.h>
#include <arpa/inet.h>

int main() {
    // Open a socket to communicate with the wireless device
    int sockfd = iw_sockets_open();
    if (sockfd < 0) {
        perror("iw_sockets_open");
        exit(EXIT_FAILURE);
    }

    // Specify the wireless interface
    const char *interface = "wlp61s0";

    // Perform the scan
    wireless_scan_head scan_head;
    wireless_scan *result;
    iwrange range;

    // Get the range information
    if (iw_get_range_info(sockfd, interface, &range) < 0) {
        perror("iw_get_range_info");
        iw_sockets_close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Perform the scan
    if (iw_scan(sockfd, interface, range.we_version_compiled, &scan_head) < 0) {
        perror("iw_scan");
        iw_sockets_close(sockfd);
        exit(EXIT_FAILURE);
    }


    // Iterate through the scan results
    result = scan_head.result;
    while (result != NULL) {
        printf("ESSID: %s\n", result->b.essid);
        struct sockaddr_in *ap_addr_in = (struct sockaddr_in *)&result->ap_addr;
        printf("Access Point Address: %s\n", inet_ntoa(ap_addr_in->sin_addr));
        printf("Frequency: %g GHz\n", iw_freq2float(&(result->b.freq)));
        printf("Signal Level: %d dBm\n", result->stats.qual.level - 0x100);
        printf("Noise Level: %d dBm\n", result->stats.qual.noise - 0x100);
        printf("Bit Rate: %d Mb/s\n", result->maxbitrate.value / 1000000);
        printf("Encryption Key: %s\n", result->b.key_flags & IW_ENCODE_DISABLED ? "off" : "on");
        printf("Mode: %d\n", result->b.mode);
        printf("\n");

        result = result->next;
    }

    // Close the socket
    iw_sockets_close(sockfd);

    return 0;
}