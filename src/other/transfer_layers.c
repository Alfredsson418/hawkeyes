#include "../../include/other/transfer_layers.h"


char * get_transfer_layer_string(transfer_protocol_t protocol) {
    switch (protocol) {
        case (TL_TCP_t):
            return "tcp";
        case (TL_UDP_t):
            return "udp";
        default:
            return "err";
    }
}

char * get_transfer_layer_by_scan(scan_methods_t method) {
    switch (method) {
        case (SCAN_TCP_t):
        case (SCAN_HALF_SYNC_t):
            return get_transfer_layer_string(TL_TCP_t);
        case (SCAN_UDP_t):
            return get_transfer_layer_string(TL_UDP_t);
        default:
            return "err";
    }
}
