#include "../../../include/scan_methods/tcp/half_sync.h"

// https://nmap.org/book/synscan.html
//
// https://linux-kernel-labs.github.io/refs/heads/master/labs/networking.html

int SYN_scan(scan_arg_t arg, scan_result_t *result) {
	struct timespec start, stop;
	unsigned int	ip_hdr_len	= arg.addr->ss_family == AF_INET
									  ? sizeof(struct iphdr)
									  : sizeof(struct ip6_hdr);
	unsigned int	packet_size = sizeof(struct tcphdr) + ip_hdr_len;
	char			packet[packet_size];
	void		   *ip_hdr	= packet;
	struct tcphdr  *tcp_hdr = (struct tcphdr *)(packet + ip_hdr_len);
	next_best_args	recv_packet_args;
	pthread_t		thread_id;
	net_packet	   *recv_packet = NULL;

	recv_packet_args.timeout		= arg.timeout;
	recv_packet_args.interface		= arg.interface->name;
	recv_packet_args.setup_complete = false;

	int sock = socket_init(SOCK_RAW, IPPROTO_RAW, arg);

	if (arg.addr->ss_family == AF_INET) {
		((struct sockaddr_in *)arg.addr)->sin_port = htons(arg.port);
		ip_hdr									   = (struct iphdr *)packet;
		ip_hdr_setup((struct iphdr *)ip_hdr,
					 (struct sockaddr_in *)&arg.interface->s_addr,
					 (struct sockaddr_in *)arg.addr, 100000,
					 sizeof(struct tcphdr));

		sprintf(recv_packet_args.filter,
				"(tcp src port %#06x) && (tcp[tcpflags] & (tcp-syn|tcp-ack) == "
				"(tcp-syn|tcp-ack))",
				arg.port);
	} else if (arg.addr->ss_family == AF_INET6) {

		((struct sockaddr_in6 *)arg.addr)->sin6_port = htons(arg.port);
		ip_hdr										 = (struct ip6_hdr *)packet;
		ip6_hdr_setup((struct ip6_hdr *)ip_hdr,
					  (struct sockaddr_in6 *)&arg.interface->s_addr,
					  (struct sockaddr_in6 *)arg.addr, 100000,
					  sizeof(struct tcphdr));

		sprintf(recv_packet_args.filter,
				"(tcp src port %#06x) && (tcp[tcpflags] & (tcp-syn|tcp-ack) == "
				"(tcp-syn|tcp-ack))",
				arg.port);
	}

	if (pthread_create(&thread_id, NULL, (void *)next_best_packet,
					   &recv_packet_args) != 0) {
		ERR_PRINT("Failed to create capture thread \n");
		return -1;
	}

	tcp_hdr_setup(tcp_hdr, 54302, arg.port, 1105024978, &arg.interface->s_addr,
				  arg.addr);

	while ((!recv_packet_args.setup_complete)) {
		;
	}
	clock_gettime(CLOCK_MONOTONIC, &start);
	sendto(sock, packet, packet_size, 0, (struct sockaddr *)arg.addr,
		   get_addr_len(arg.addr));

	pthread_join(thread_id, (void **)&recv_packet);
	clock_gettime(CLOCK_MONOTONIC, &stop);
	result->scannig_time = time_in_x(start, stop, NANO);

	if (recv_packet == NULL) {
		result->state = -1;

		// This needs the && because the first check is for if its empty or not
		// The second is only run when its not empty
	} else if (recv_packet->packet_header != NULL &&
			   recv_packet->packet_header->len > 0) {
		free(recv_packet->packet_payload);
		free(recv_packet->packet_header);
		free(recv_packet);
		result->state = 1;
	} else {
		result->state = 0;
	}

	return 0;
}
