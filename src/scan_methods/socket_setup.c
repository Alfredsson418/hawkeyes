#include "../../include/scan_methods/socket_setup.h"

int socket_init(int protocol, scan_arg_t func_arg) {

    struct timeval timeout;
    timeout.tv_sec  = func_arg.timeout;
    timeout.tv_usec = 0; // Should be easy to and more specific timeout

    int sock = socket(func_arg.addr->ss_family, protocol, 0);

    if (sock < 0) {
        ERR_PRINT("Failed to create TCP socket \n");
        return -1;
    }

    if (setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (const char *)&timeout,
                   sizeof(timeout)) < 0) {
        ERR_PRINT("TCP Setup timeout send \n");
        close(sock);
        return -1;
    }

    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout,
                   sizeof(timeout)) < 0) {
        ERR_PRINT("TCP Setup timeout rcv \n");
        close(sock);
        return -1;
    }

    if (setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE,
                   func_arg.network_interface, INTERFACE_LEN) < 0) {
        ERR_PRINT("TCP Setup interface \n");
        close(sock);
        return -1;
    }

    return sock;
}

void socket_close(int socket) { close(socket); }

unsigned int get_addr_len(struct sockaddr_storage *addr) {

    if (addr->ss_family == AF_INET) {
        return sizeof(struct sockaddr_in);
    } else if (addr->ss_family == AF_INET6) {
        return sizeof(struct sockaddr_in6);
    }
    ERR_PRINT("Unknown address family\n");
    return -1;
}
