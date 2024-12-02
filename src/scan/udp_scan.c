#include "../../include/scan/udp_scan.h"


// Define a struct that holds the arguments for run_next_best_packet
typedef struct {
    int timeout;
    char filter[80];
    char * device;
    bool setup_complete;
} next_best_args;

void * run_next_best_packet(void * arg) {
    next_best_args* args = (next_best_args*)arg;

    return (void *)next_best_packet(args->device, args->filter, args->timeout, &args->setup_complete);
}

int udp_scan(scan_arg_t arg, scan_result_t * result) {
    /*
        The method in this funtion to find open ports it to
        scan for "Port Unreachable" or "Destination Unreachable"
        ICMP replys when sending a random package to the( target
        If there is no respone, either the port is open or the
        firewall was configured not to responde. This function
        wont wait to fetch the responce because the ICMP package
        wont always be given to the program.
    */

    pthread_t thread_id;
    struct timespec start, stop;
    net_packet * packet = NULL;
    result->method = 0;

    /*
    Create a struct that holds the arguments for run_next_best_packet
    next_best_args* args = calloc(1, sizeof(next_best_args));
    args->packet = calloc(MAX_PACKET_SIZE + 1, sizeof(char));
    args->packet_header = calloc(1, sizeof(struct pcap_pkthdr));
    */
    next_best_args packet_capture_arg;

    packet_capture_arg.timeout = arg.timeout;
    packet_capture_arg.device = arg.network_interface;
    packet_capture_arg.setup_complete = false;
    // strcpy(packet_capture_arg->device, arg.network_interface);

    /*
        (icmp[30:2] == %#06x): This condition checks if the src port of the ICMP packet SENT
        matches the port variable. The %#06x format specifier ensures that
        the port value is formatted as a zero-padded, 6-character wide
        hexadecimal number with a 0x prefix.

        (icmp[0] == 3): This condition checks if the first byte of the ICMP
        packet is equal to 3, which typically indicates a "Destination Unreachable"
        message in ICMP.

        (icmp[1] == 3): This condition checks if the second byte of the
        ICMP packet is equal to 3, which usually specifies the "Port Unreachable"
        code within the "Destination Unreachable" message.
    */
    sprintf(packet_capture_arg.filter, "(icmp[30:2] == %#06x) && (icmp[0] == 3) && (icmp[1] == 3)", arg.port);

    // PRINT("%s\n", packet_capture_arg->filter);

    if (pthread_create(&thread_id, NULL, run_next_best_packet, &packet_capture_arg) != 0) {
        ERR_PRINT("Failed to create capture thread \n");
        return -1;
    }

    int sock = socket(AF_INET, SOCK_DGRAM, 0);

    struct timeval timeout;
    timeout.tv_sec = arg.timeout;
    timeout.tv_usec = 0;

    if (setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout)) < 0) {
        ERR_PRINT("TCP Setup timeout send error \n");
        close(sock);
        result->state = -1;
        return result->state;
    }

    if ( setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout)) < 0) {
        ERR_PRINT("TCP Setup timeout rcv error \n");
        close(sock);
        result->state = -1;
        return result->state;
    }

    if (setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, arg.network_interface, INTERFACE_LEN) < 0) {
        ERR_PRINT("TCP Setup timeout send error \n");
        close(sock);
        result->state = -1;
        return result->state;
    }


    if (sock < 0) {
        ERR_PRINT("Failed to create UDP socket \n");
        result->state = -1;
        return result->state;
    }

    struct sockaddr_in addr;

    memset(&addr, 0, sizeof(addr));


    addr.sin_family = AF_INET;
    addr.sin_port = htons(arg.port);
    addr.sin_addr = arg.ipv4;

    // This needs to be here so that the scanning can start before the package is sent
    // Could also use a varible that checks if the function has started scanning
    // This could set the program in an infinite loop, but has not happened yet.
    while((!packet_capture_arg.setup_complete)) {
        ;
    }

    clock_gettime(CLOCK_MONOTONIC, &start);
    if (sendto(sock, 0, 0, 0, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        ERR_PRINT("Failed to send UDP package");
        close(sock);
        result->state = -1;
        return result->state;
    }

    pthread_join(thread_id, (void **)&packet);
    clock_gettime(CLOCK_MONOTONIC, &stop);

    result->scannig_time = time_in_x(start, stop, NANO_S);

    if (packet == NULL) {
        ERR_PRINT("Failed to capture UDP packet\n");
        result->state = -1;

        // This needs the && because the first check is for if its empty or not
        // The second is only run when its not empty
    }else if (packet->packet_header != NULL && packet->packet_header->len > 0) {
        free(packet->packet_payload);
        free(packet->packet_header);
        free(packet);
        result->state = 0;
    } else {
        result->state = 1;
    }
    close(sock);

    return result->state;
}
