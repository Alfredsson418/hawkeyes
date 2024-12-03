#include "../../include/other/port_scan_loopup.h"


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
                return "ECHO Reply";
        }
    }

    return "Err";
}
