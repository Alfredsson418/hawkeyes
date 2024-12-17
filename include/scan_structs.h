#pragma once

#include <arpa/inet.h>

#include "constans.h"

// #include "constans.h"

typedef struct {
    int state;
    int method;
    double scannig_time; // Scanning time for port
}scan_result_t;

typedef struct {
    struct sockaddr_storage address;
    char network_interface[INTERFACE_LEN];
    unsigned int timeout;
    unsigned short port;
}scan_arg_t;


typedef enum {
    SCAN_TCP_t = 0,
    SCAN_UDP_t = 1,
    SCAN_HALF_SYNC_t = 2,
}scan_methods_t;
