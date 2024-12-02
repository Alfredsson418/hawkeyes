#pragma once

#include "../../include/netspectre.h"
#include <semaphore.h>

typedef struct {
    struct in_addr target;
    int index;
    int * ports;
    int * ports_result;
    int port_len;
    int time_intervals;
    pthread_mutex_t * write_mutex;
    pthread_mutex_t * read_mutex;
    int (*function)(scan_function_arguments);
    scan_function_arguments in_arg;
}worker_arg;

int * multithread_scanning(int max_workers, int * ports, int port_len, void * function, scan_function_arguments function_arg);
