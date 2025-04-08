#include "../../../include/scan_methods/udp/echo_responce.h"

void *run_next_best_packet(void *arg) {
	next_best_args *args = (next_best_args *)arg;

	return (void *)next_best_packet(args);
}

/*
	The method in this funtion to find open ports it to
	scan for "Port Unreachable" or "Destination Unreachable"
	ICMP replys when sending a random package to the( target
	If there is no respone, either the port is open or the
	firewall was configured not to responde. This function
	wont wait to fetch the responce because the ICMP package
	wont always be given to the program.
*/
int icmp_responce_scan(scan_arg_t arg, scan_result_t *result) {

	struct timespec start, stop;
	clock_gettime(CLOCK_MONOTONIC, &start);

	pthread_t	thread_id;
	net_packet *packet = NULL;
	int			sock   = socket_init(SOCK_DGRAM, 0, arg);
	if (sock < 0) {
		return -1;
	}

	next_best_args packet_capture_arg;

	packet_capture_arg.timeout		  = arg.timeout;
	packet_capture_arg.interface	  = arg.interface->name;
	packet_capture_arg.setup_complete = false;

	if (arg.addr->ss_family == AF_INET) {
		((struct sockaddr_in *)arg.addr)->sin_port = htons(arg.port);
		sprintf(packet_capture_arg.filter,
				"(icmp[30:2] == %#06x) && (icmp[0] == 3) && (icmp[1] == 3)",
				arg.port);
	} else if (arg.addr->ss_family == AF_INET6) {
		((struct sockaddr_in6 *)arg.addr)->sin6_port = htons(arg.port);
		sprintf(packet_capture_arg.filter,
				"(icmp6[50:2] == %#06x) && (icmp6[0] == 1) && (icmp6[1] == 4)",
				arg.port);
	}

	if (pthread_create(&thread_id, NULL, run_next_best_packet,
					   &packet_capture_arg) != 0) {
		ERR_PRINT("Failed to create capture thread \n");
		return -1;
	}

	// This needs to be here so that the scanning can start before the package
	// is sent Could also use a varible that checks if the function has started
	// scanning This could set the program in an infinite loop, but has not
	// happened yet.
	while ((!packet_capture_arg.setup_complete)) {
		;
	}

	if (sendto(sock, 0, 0, 0, (struct sockaddr *)arg.addr,
			   get_addr_len(arg.addr)) < 0) {
		ERR_PRINT("Failed to send UDP package\n");
		result->state = -1;
		return -2;
	}

	pthread_join(thread_id, (void **)&packet);

	if (packet == NULL) {
		// ERR_PRINT("Failed to capture UDP packet\n");
		result->state = -1;

		// This needs the && because the first check is for if its empty or not
		// The second is only run when its not empty
	} else if (packet->packet_header != NULL &&
			   packet->packet_header->len > 0) {
		free(packet->packet_payload);
		free(packet->packet_header);
		free(packet);
		result->state = 0;
	} else {
		result->state = 1;
	}

	socket_close(sock);

	clock_gettime(CLOCK_MONOTONIC, &stop);
	result->scannig_time = time_in_x(start, stop, NANO);

	return result->state;
}
