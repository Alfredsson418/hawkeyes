#pragma once

// Could possibly add more in the future
// https://en.wikipedia.org/wiki/Transport_layer

#include "../scan_structs.h"

typedef enum {
    TL_TCP_t = 0,
    TL_UDP_t = 1,
} transfer_protocol_t;

char * get_transfer_layer_string(transfer_protocol_t protocol);

char * get_transfer_layer_by_scan(scan_methods_t method);
