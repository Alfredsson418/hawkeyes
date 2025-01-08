#include "../../../include/scan_methods/tcp/connect.h"

int connect_scan(scan_arg_t arg, scan_result_t *result) {
    struct timespec start, stop;

    int sock = socket_init(SOCK_STREAM, arg);

    clock_gettime(CLOCK_MONOTONIC, &start);

    if (arg.addr->ss_family == AF_INET) {
        ((struct sockaddr_in *)arg.addr)->sin_port = htons(arg.port);
    } else if (arg.addr->ss_family == AF_INET6) {
        ((struct sockaddr_in6 *)arg.addr)->sin6_port = htons(arg.port);
    }

    if (connect(sock, (struct sockaddr *)arg.addr, get_addr_len(arg.addr)) <
        0) {
        // No connection
        result->state = 0;
    } else {
        // Connected
        result->state = 1;
    }

    socket_close(sock);

    clock_gettime(CLOCK_MONOTONIC, &stop);

    result->scannig_time = time_in_x(start, stop, NANO);

    return result->state;
}
