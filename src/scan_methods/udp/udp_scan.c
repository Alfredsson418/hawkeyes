#include "../../../include/scan_methods/udp/udp_scan.h"


int udp_scan(scan_arg_t arg, scan_result_t * result) {
    struct timespec start, stop;

    clock_gettime(CLOCK_MONOTONIC, &start);
    int scan_result = udp_echo_responce(arg, result);
    clock_gettime(CLOCK_MONOTONIC, &stop);

    result->scannig_time = time_in_x(start, stop, NANO);

    return scan_result;

}
