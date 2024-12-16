#include "../../include/other/port_scan_lookup.h"

int calculate_scanned(scan_result_t * a, int n) {
    double scanned = 0;
    for (int i = 0; i < n; i++) {
        if (a[i].state >= 0) {
            scanned++;
        }
    }
    return scanned;
}


char * state_string(int state) {
    switch (state) {
        case (0):
        return "Closed";
        case (1):
        return "Open";
        default:
        return "Err";
    }
}

char * scanning_method(scan_methods_t scan_method, int method) {
    if (scan_method == SCAN_TCP_t) {
        switch (method) {
            case (0):
                return "Three-way Handshake";
        }
    }

    if (scan_method == SCAN_UDP_t) {
        switch (method) {
            case (0):
                return "No ECHO Reply";
        }
    }

    return "Err";
}
