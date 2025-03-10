#include "../../include/scan_methods/socket_setup.h"

int socket_init(int socket_type, int protocol, scan_arg_t func_arg) {

	struct timeval timeout;
	timeout.tv_sec	= func_arg.timeout;
	timeout.tv_usec = 0; // Should be easy to and more specific timeout

	int sock = socket(func_arg.addr->ss_family, socket_type, protocol);

	if (sock < 0) {
		ERR_PRINT("Failed to create TCP socket \n");
		return -1;
	}

	if (setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (const char *)&timeout,
				   sizeof(timeout)) < 0) {
		ERR_PRINT("TCP Setup timeout send \n");
		close(sock);
		return -1;
	}

	if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout,
				   sizeof(timeout)) < 0) {
		ERR_PRINT("TCP Setup timeout rcv \n");
		close(sock);
		return -1;
	}

	if (setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, func_arg.interface->name,
				   INTERFACE_LEN) < 0) {
		ERR_PRINT("TCP Setup interface \n");
		close(sock);
		return -1;
	}

	return sock;
}

void socket_close(int socket) { close(socket); }

unsigned int get_addr_len(struct sockaddr_storage *addr) {

	if (addr->ss_family == AF_INET) {
		return sizeof(struct sockaddr_in);
	} else if (addr->ss_family == AF_INET6) {
		return sizeof(struct sockaddr_in6);
	}
	ERR_PRINT("Unknown address family\n");
	return -1;
}

unsigned short random_port() {
	return (rand() % (RAND_PORT_LOWER_LIMIT - RAND_PORT_UPPER_LIMIT + 1) +
			(RAND_PORT_LOWER_LIMIT));
}

unsigned short unique_port(int domain) {
	while (1) {
		int				   sockfd;
		struct sockaddr_in addr;
		unsigned short	   port = random_port();

		// Create a socket
		sockfd = socket(AF_INET, SOCK_DGRAM, 0);
		if (sockfd < 0) {
			perror("socket");
			continue;
		}

		// Prepare the sockaddr_in structure
		memset(&addr, 0, sizeof(addr));
		addr.sin_family		 = domain;
		addr.sin_addr.s_addr = INADDR_ANY;
		addr.sin_port		 = htons(port);

		// Try binding to the port
		int bind_result = bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));
		if (bind_result >= 0) {
			close(sockfd);
			return port;
		}
		close(sockfd);
	}
}
