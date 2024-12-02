#include "../../include/scan/multithread_scanning.h"

void *worker_function(void *in_arg) {
    worker_arg * arg = in_arg;

    while (1) {
        pthread_mutex_lock(arg->read_mutex);
        if (arg->index >= arg->port_len) { break; }
        int current_index = arg->index;
        arg->index++;
        int port = *(arg->ports + current_index);
        pthread_mutex_unlock(arg->read_mutex);

        arg->in_arg.port = port;
        arg->function(arg->in_arg, (arg->result + current_index));

        pthread_mutex_lock(arg->write_mutex);
        pthread_mutex_unlock(arg->write_mutex);
    }
    pthread_mutex_unlock(arg->read_mutex);
    return 0;
}

int multithread_scanning(unsigned int max_workers, unsigned short * ports, unsigned short port_len, void * function, scan_arg_t function_arg, scan_result_t * result) {
    pthread_mutex_t read_mutex;
    pthread_mutex_t write_mutex;
    worker_arg in_arg;
    pthread_t workers[max_workers];


    pthread_mutex_init(&read_mutex, NULL);
    pthread_mutex_init(&write_mutex, NULL);

    in_arg.index = 0;
    in_arg.ports = ports;
    in_arg.port_len = port_len;
    in_arg.read_mutex = &read_mutex;
    in_arg.write_mutex = &read_mutex;
    in_arg.function = function;
    in_arg.in_arg = function_arg;
    in_arg.result = result;

    for(int i = 0; i < max_workers; i++) {
        pthread_create(&(workers[i]), NULL, worker_function, &in_arg);
    }

    for(int i = 0; i < max_workers; i++) {
        pthread_join(workers[i], NULL);
    }

    pthread_mutex_destroy(&read_mutex);
    pthread_mutex_destroy(&write_mutex);
    return 0;
}
