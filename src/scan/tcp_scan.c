#include "../../include/scan/tcp_scan.h"

int tcp_scan(scan_arg_t arg, scan_result_t * result) {
    struct timespec start, stop;
    result->method = 0;

    int sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0) {
        ERR_PRINT("Failed to create TCP socket \n");
        result->state = -1;
        return result->state;
    }

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


    struct sockaddr_in addr;

    memset(&addr, 0, sizeof(struct sockaddr_in));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(arg.port);
    addr.sin_addr = arg.ipv4;

    clock_gettime(CLOCK_MONOTONIC, &start);
    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        // No connection
        result->state = 0;
    } else {
        // Connected
        result->state = 1;
    }
    clock_gettime(CLOCK_MONOTONIC, &stop);

    result->scannig_time = time_in_x(start, stop, NANO_S);

    close(sock);
    return result->state;

}
