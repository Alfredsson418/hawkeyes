#include "../../include/scan_methods/multithread_scanning.h"
#include <termios.h>
#include <fcntl.h>

typedef struct{
    scan_result_t * result;
    int result_len;
    bool stop;
}waiting_t;

void set_nonblocking_mode() {
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);

    // Set terminal to non-canonical mode and disable echo
    tty.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);

    // Set file descriptor to non-blocking mode
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
}

void reset_terminal_mode() {
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);

    // Restore terminal to canonical mode and enable echo
    tty.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);

    // Restore file descriptor to blocking mode
    fcntl(STDIN_FILENO, F_SETFL, 0);
}


void * waiting_on_scan(void * wait) {
    waiting_t *waiting_data = (waiting_t *)wait;
    struct timespec start, stop;
    clock_gettime(CLOCK_MONOTONIC, &start);
    set_nonblocking_mode();
    while (!(waiting_data->stop)) {
        // Check if Enter key is pressed
        int ch = getchar(); // needs to be non_blocking
        if (ch == '\n') {
            clock_gettime(CLOCK_MONOTONIC, &stop);
            double open = calculate_scanned(waiting_data->result, waiting_data->result_len);
            PRINT("Time elapes: %.2f sec   Total port scanned: %.1f%%\n", time_in_x(start, stop, SECONDS), (open/waiting_data->result_len) * 100);

        }
        usleep(100000);
    }
    reset_terminal_mode();
    return NULL;
}

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
    pthread_t waiting_thread;


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

    waiting_t wait;
    wait.result = result;
    wait.result_len = port_len;
    wait.stop = false;

    for(int i = 0; i < max_workers; i++) {
        pthread_create(&(workers[i]), NULL, worker_function, &in_arg);
    }

    pthread_create(&waiting_thread, NULL, waiting_on_scan, (void *)&wait);

    for(int i = 0; i < max_workers; i++) {
        pthread_join(workers[i], NULL);
    }

    wait.stop = true;
    pthread_join(waiting_thread, NULL);

    pthread_mutex_destroy(&read_mutex);
    pthread_mutex_destroy(&write_mutex);
    return 0;
}
