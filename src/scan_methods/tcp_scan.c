#include "../../include/scan_methods/tcp_scan.h"
#include <netinet/in.h>

int tcp_scan_init(scan_arg_t arg) {

    struct timeval timeout;
    timeout.tv_sec = arg.timeout;
    timeout.tv_usec = 0;

    struct sockaddr *addr = (struct sockaddr *)&arg.address;

    int sock = socket(addr->sa_family, SOCK_STREAM, 0);

    if (sock < 0) {
        ERR_PRINT("Failed to create TCP socket \n");
        return -1;
    }

    if (setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout)) < 0) {
        ERR_PRINT("TCP Setup timeout send error \n");
        close(sock);
        return -1;
    }

    if ( setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout)) < 0) {
        ERR_PRINT("TCP Setup timeout rcv error \n");
        close(sock);
        return -1;
    }

    if (setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, arg.network_interface, INTERFACE_LEN) < 0) {
        ERR_PRINT("TCP Setup timeout send error \n");
        close(sock);
        return -1;
    }

    return sock;
}

int tcp_scan(int sock, scan_arg_t arg, scan_result_t * result) {
    struct timespec start, stop;
    result->method = 0;
    void * addr;

    if (arg.address.ss_family == AF_INET) {
        addr = (struct sockaddr_in *)&arg.address;
    } else if (arg.address.ss_family == AF_INET6) {
        addr = (struct sockaddr_in6 *)&arg.address;
    } else {
        ERR_PRINT("Wrong addr format\n");
        return -1;
    }

    clock_gettime(CLOCK_MONOTONIC, &start);
    if (connect(sock, addr, sizeof(*addr)) < 0) {
        // No connection
        result->state = 0;
    } else {
        // Connected
        result->state = 1;
    }
    clock_gettime(CLOCK_MONOTONIC, &stop);

    result->scannig_time = time_in_x(start, stop, NANO);

    return result->state;
}

void tcp_scan_free(int sock) {
    close(sock);
}
