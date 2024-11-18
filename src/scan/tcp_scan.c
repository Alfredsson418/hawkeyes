#include "../../include/scan/tcp_scan.h"

int tcp_scan(scan_function_arguments arg) {

    int sock = socket(AF_INET, SOCK_STREAM, 0);


    if (sock < 0) {
        ERR_PRINT("Failed to create TCP socket \n");
        return -1;
    }

    struct timeval timeout;
    // Set the timeout value for receiving (SO_RCVTIMEO) and sending (SO_SNDTIMEO)
    timeout.tv_sec = arg.timeout;
    timeout.tv_usec = 0; // 0 microseconds
    int ret;
    ret = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
    if (ret < 0) {
        ERR_PRINT("TCP Setup timeout error \n");
        close(sock);
        return -1;
    }

    struct sockaddr_in addr;

    memset(&addr, 0, sizeof(struct sockaddr_in));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(arg.port);
    addr.sin_addr = arg.ipv4;
    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        // No connection
        close(sock);
        return 0;
    }
    close(sock);
    return 1;
}
