#include "../../include/scan/udp_scan.h"


// Define a struct that holds the arguments for run_next_best_packet
typedef struct {
    int timeout;
    char filter[80];
    char * device;
} next_best_args;

void * run_next_best_packet(void * arg) {
    next_best_args* args = (next_best_args*)arg;

    return (void *)next_best_packet(args->device, args->filter, args->timeout);
}


int udp_scan(scan_function_arguments arg) {
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
    /*
    Create a struct that holds the arguments for run_next_best_packet
    next_best_args* args = calloc(1, sizeof(next_best_args));
    args->packet = calloc(MAX_PACKET_SIZE + 1, sizeof(char));
    args->packet_header = calloc(1, sizeof(struct pcap_pkthdr));
    */
    next_best_args * packet_capture_arg = calloc(1, sizeof(next_best_args));

    packet_capture_arg->timeout = arg.timeout;
    packet_capture_arg->device = arg.network_interface;
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
    sprintf(packet_capture_arg->filter, "(icmp[30:2] == %#06x) && (icmp[0] == 3) && (icmp[1] == 3)", arg.port);

    // PRINT("%s\n", packet_capture_arg->filter);

    if (pthread_create(&thread_id, NULL, run_next_best_packet, packet_capture_arg) != 0) {
        ERR_PRINT("Failed to create capture thread \n");
        return -1;
    }

    int sock = socket(AF_INET, SOCK_DGRAM, 0);


    if (sock < 0) {
        ERR_PRINT("Failed to create UDP socket \n");
        return -1;
    }

    struct sockaddr_in addr;

    memset(&addr, 0, sizeof(addr));


    addr.sin_family = AF_INET;
    addr.sin_port = htons(arg.port);
    addr.sin_addr = arg.ipv4;

    // This needs to be here so that the scanning can start before the package is sent
    // Could also use a varible that checks if the function has started scanning
    sleep(2);

    if (sendto(sock, 0, 0, 0, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        ERR_PRINT("Failed to send UDP package");
        close(sock);
        return -1;
    }

    net_packet * packet = NULL;
    pthread_join(thread_id, (void **)&packet);

    int result;
    if (packet == NULL) {
        ERR_PRINT("Failed to capture UDP packet");
        result = -1;

        // This needs the && because the first check is for if its empty or not
        // The second is only run when its not empty
    }else if (packet->packet_header != NULL && packet->packet_header->len > 0) {
        free(packet->packet_payload);
        free(packet->packet_header);
        free(packet);
        result = 0;
    } else {
        result = 1;
    }
    close(sock);
    // free_dev(packet_capture_arg->device);
    free(packet_capture_arg);

    return result;
}
