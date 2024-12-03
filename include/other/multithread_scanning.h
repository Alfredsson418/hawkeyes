#pragma once

#include <semaphore.h>
#include <pthread.h>
#include <arpa/inet.h>

#include "../scan_structs.h"

typedef struct {
    struct in_addr target;
    short unsigned int index;
    unsigned short * ports;
    unsigned short port_len;
    unsigned int time_intervals;
    pthread_mutex_t * write_mutex;
    pthread_mutex_t * read_mutex;
    int (*function)(scan_arg_t, scan_result_t *);
    scan_arg_t in_arg;
    scan_result_t * result;
}worker_arg;

int multithread_scanning(unsigned int max_workers, unsigned short * ports, unsigned short port_len, void * function, scan_arg_t function_arg, scan_result_t * result);
