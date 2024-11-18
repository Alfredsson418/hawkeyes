#include "../../include/scan/multithread_scanning.h"

void *consumer_function(void *in_arg) {

    consumer_arg * arg = in_arg;

    while (1) {
        pthread_mutex_lock(arg->read_mutex);
        if (arg->index >= arg->port_len) { break; }
        int current_index = arg->index;
        arg->index++;
        int port = *(arg->ports + current_index);
        pthread_mutex_unlock(arg->read_mutex);

        arg->in_arg.port = port;
        int func_return = arg->function(arg->in_arg);

        pthread_mutex_lock(arg->write_mutex);
        *(arg->ports_result + current_index) = func_return;

        pthread_mutex_unlock(arg->write_mutex);
    }
    pthread_mutex_unlock(arg->read_mutex);
    return 0;
}

int * multithread_scanning(int max_consumers, int * ports, int port_len, void * function, scan_function_arguments function_arg) {
    pthread_mutex_t read_mutex;
    pthread_mutex_t write_mutex;
    pthread_mutex_init(&read_mutex, NULL);
    pthread_mutex_init(&write_mutex, NULL);

    consumer_arg * in_arg = calloc(1, sizeof(consumer_arg));
    in_arg->index = 0;
    in_arg->ports = ports;
    in_arg->ports_result = calloc(sizeof(unsigned int), port_len);
    // 2 = did not scan
    // 1 = possibly open port
    // 0 = not open
    // -1 = an error occured
    for (int i = 0; i < port_len; i++) {
        *(in_arg->ports_result + i) = 2;
    }
    in_arg->port_len = port_len;
    in_arg->read_mutex = &read_mutex;
    in_arg->write_mutex = &read_mutex;
    in_arg->function = function;
    in_arg->in_arg = function_arg;


    pthread_t consumers[max_consumers];
    for(int i = 0; i < max_consumers; i++) {
        pthread_create(&(consumers[i]), NULL, consumer_function, in_arg);
    }

    for(int i = 0; i < max_consumers; i++) {
        pthread_join(consumers[i], NULL);
    }



    printf("Exits program\n");

    pthread_mutex_destroy(&read_mutex);
    pthread_mutex_destroy(&write_mutex);
    return in_arg->ports_result;
}
