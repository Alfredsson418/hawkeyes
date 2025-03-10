#include "../../include/scan_methods/multithread_scanning.h"

typedef struct {
	scan_result_t *result;
	unsigned int   result_len;
	bool		   stop;
} waiting_t;

typedef struct {
	unsigned int   *index;
	unsigned short *ports;
	unsigned int	port_len; // Not unsigned short to not get a buffer overflow
	pthread_mutex_t *read_mutex;
	scan_func_t		 func_info;
	scan_arg_t		 func_arg;
	scan_result_t	*func_result;
} worker_arg;

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

void *waiting_on_scan(void *wait) {
	waiting_t	   *waiting_data = (waiting_t *)wait;
	struct timespec start, stop;

	clock_gettime(CLOCK_MONOTONIC, &start);
	set_nonblocking_mode();
	while (!(waiting_data->stop)) {
		// Check if Enter key is pressed
		int ch = getchar(); // needs to be non_blocking
		if (ch == '\n') {
			clock_gettime(CLOCK_MONOTONIC, &stop);
			double open = calculate_scanned(waiting_data->result,
											waiting_data->result_len);
			PRINT("Time elapes: %.2f sec   Total port scanned: %.1f%%\n",
				  time_in_x(start, stop, SECONDS),
				  (open / waiting_data->result_len) * 100);
		}
		usleep(100000);
	}
	reset_terminal_mode();
	return NULL;
}

void *worker_function(void *in_arg) {
	worker_arg *temp_arg = in_arg;
	worker_arg	arg		 = *temp_arg; // Look at issue 21 on github for why

	// PRINT("Thread start\n");
	while (1) {

		pthread_mutex_lock(arg.read_mutex);
		if (*arg.index >= arg.port_len) {
			// PRINT("EXITS %d %d\n", *arg.index, arg.port_len);
			break;
		}
		int current_index = *arg.index;
		(*arg.index)++;
		int port = *(arg.ports + current_index);
		pthread_mutex_unlock(arg.read_mutex);

		arg.func_arg.port = port;
		// PRINT("Trying to scan on %d\n", port);
		arg.func_info.scan_func(arg.func_arg,
								(arg.func_result + current_index));
		// if (arg.func_result[current_index].state > 0) {
		//     PRINT("Open port on %d\n", port);
		// }
	}
	pthread_mutex_unlock(arg.read_mutex);

	return 0;
}

int multithread_scanning(scan_func_t *func_info, scan_arg_t *func_arg,
						 scan_result_t *func_result, unsigned short *ports,
						 unsigned int port_len, unsigned int workers) {
	pthread_mutex_t read_mutex;
	worker_arg		in_arg;
	pthread_t		pthread_workers[workers];
	pthread_t		waiting_thread;
	waiting_t		wait;

	if (func_info->needs_root && !is_root()) {
		ERR_PRINT("Permission Denied\n");
		return -1;
	}

	pthread_mutex_init(&read_mutex, NULL);

	in_arg.index = malloc(sizeof(unsigned int));
	if (in_arg.index == NULL) {
		ERR_PRINT("Allocating memory for scanning\n");
		return -1;
	}
	*in_arg.index	   = 0;
	in_arg.ports	   = ports;
	in_arg.port_len	   = port_len;
	in_arg.read_mutex  = &read_mutex;
	in_arg.func_info   = *func_info;
	in_arg.func_arg	   = *func_arg;
	in_arg.func_result = func_result;

	wait.result		= func_result;
	wait.result_len = port_len;
	wait.stop		= false;

	for (int i = 0; i < workers; i++) {
		pthread_create(&(pthread_workers[i]), NULL, worker_function, &in_arg);
	}

	pthread_create(&waiting_thread, NULL, waiting_on_scan, (void *)&wait);

	for (int i = 0; i < workers; i++) {
		pthread_join(pthread_workers[i], NULL);
	}

	wait.stop = true;
	pthread_join(waiting_thread, NULL);

	pthread_mutex_destroy(&read_mutex);

	free(in_arg.index);

	return 0;
}
