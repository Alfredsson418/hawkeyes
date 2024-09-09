#include "../../include/scan/udp_scan.h"
/*
    As per my understanding, creating a UDP port scanner may take some time
    due to that it is hard to know if a port is open or not.
    The service may or may not responde, and you may or my not (dependent on OS)
    get an ICMP "Port unreachable" message in return.
*/
/*
    I could you the packet sniffer to find ICMP packages with the dst port
    number maching the searching port after a seach is created, then I can find
    out if the ICMP package is sent or not. In theory
*/

// Define a struct that holds the arguments for run_next_best_packet
typedef struct {
    int timeout;
    char filter[60];
    char device[60];
} next_best_args;

void * run_next_best_packet(void * arg) {
    next_best_args* args = (next_best_args*)arg;

    return (void *)next_best_packet(args->device, args->filter, args->timeout);
}


int udp_scan(char ip[IPV4_ADDR_STR_LEN], int port, int timeout) {

    pthread_t thread_id;
    /*
    // Create a struct that holds the arguments for run_next_best_packet
    next_best_args* args = calloc(1, sizeof(next_best_args));
    args->packet = calloc(MAX_PACKET_SIZE + 1, sizeof(char));
    args->packet_header = calloc(1, sizeof(struct pcap_pkthdr));
    */
    next_best_args * args = calloc(1, sizeof(next_best_args));

    args->timeout = timeout;
    strcpy(args->device, get_net_dev_by_ip(ip));
    // First is the udp dst port, second and this is too check if it is dst and por unrech
    sprintf(args->filter, "(icmp[30:2] == %#06x) && (icmp[0] == 3) && (icmp[1] == 3)", port);

    PRINT("%s\n", args->filter);
    if (pthread_create(&thread_id, NULL, run_next_best_packet, args) != 0) {
        ERR_PRINT("%s\n", "Failed to create thread");
        return 1;
    }

    int sock = socket(AF_INET, SOCK_DGRAM, 0);

    if (sock < 0) {
        ERR_PRINT("%s\n", "ERROR creating TCP socket!");
        return -1;
    }

    struct sockaddr_in addr;

    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &addr.sin_addr) <= 0) {
        ERR_PRINT("%s\n", "Invalid address/ Address not supported");
        return -1;
    }

    // This needs to be here so that the scanning can start before the package is sent
    // Could also use a varible that checks if the function has started scanning
    sleep(1);

    if (sendto(sock, 0, 0, 0, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        ERR_PRINT("%s\n", "ERROR SENDING UDP PACKAGE");
        close(sock);
        return -1;
    }
    PRINT("%s\n", "Package Send!");
    net_packet * packet = NULL;
    pthread_join(thread_id, &packet);

    if (packet->packet_header->len > 0) {
        PRINT("%s\n", "ICMP PACKET FOUND, PORT CLOSED");
        free(packet->packet_payload);
        free(packet->packet_header);
        free(packet);
    } else {
        PRINT("%s\n", "NO PACKET RECEIVED, PORT COULD BE OPEN");
    }
    close(sock);
    free_dev(args->device);
    free(args);


    return 0;
}