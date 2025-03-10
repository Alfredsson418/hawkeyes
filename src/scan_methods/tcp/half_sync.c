#include "../../../include/scan_methods/tcp/half_sync.h"

// https://nmap.org/book/synscan.html
//
// https://linux-kernel-labs.github.io/refs/heads/master/labs/networking.html

int SYN_scan(scan_arg_t arg, scan_result_t *result) {
	struct timespec start, stop;
	clock_gettime(CLOCK_MONOTONIC, &start);
	unsigned int   ip_hdr_len  = arg.addr->ss_family == AF_INET
									 ? sizeof(struct iphdr)
									 : sizeof(struct ip6_hdr);
	unsigned int   packet_size = sizeof(struct tcphdr) + ip_hdr_len;
	char		   packet[packet_size];
	void		  *ip_hdr  = packet;
	struct tcphdr *tcp_hdr = (struct tcphdr *)(packet + ip_hdr_len);
	next_best_args recv_packet_args;
	pthread_t	   thread_id;
	net_packet	  *recv_packet = NULL;
	unsigned short src_port	   = unique_port(arg.addr->ss_family);

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

		char src_ip[INET_ADDRSTRLEN], dst_ip[INET_ADDRSTRLEN];

		ipv4_to_str(arg.addr, src_ip);
		ipv4_to_str(&arg.interface->s_addr, dst_ip);

		sprintf(recv_packet_args.filter,
				"(tcp src port %#06x) && (tcp dst port %#06x) && (ip src %s) "
				"&& (ip dst %s)",
				arg.port, src_port, dst_ip,
				src_ip); // Src and Dst switches places
	} else if (arg.addr->ss_family == AF_INET6) {
		((struct sockaddr_in6 *)arg.addr)->sin6_port = htons(arg.port);
		ip_hdr										 = (struct ip6_hdr *)packet;
		ip6_hdr_setup((struct ip6_hdr *)ip_hdr,
					  (struct sockaddr_in6 *)&arg.interface->s_addr,
					  (struct sockaddr_in6 *)arg.addr, 100000,
					  sizeof(struct tcphdr));

		char src_ip[INET6_ADDRSTRLEN], dst_ip[INET6_ADDRSTRLEN];

		ipv6_to_str(arg.addr, src_ip);
		ipv6_to_str(&arg.interface->s_addr, dst_ip);

		sprintf(recv_packet_args.filter,
				"(tcp src port %#06x) && (tcp dst port %#06x) && (ip6 src %s) "
				"&& (ip6 dst %s)",
				arg.port, src_port, dst_ip, src_ip);
	}

	if (pthread_create(&thread_id, NULL, (void *)next_best_packet,
					   &recv_packet_args) != 0) {
		ERR_PRINT("Failed to create capture thread \n");
		socket_close(sock);
		return -1;
	}

	tcp_hdr_setup(tcp_hdr, src_port, arg.port, 1105024978,
				  &arg.interface->s_addr, arg.addr);
	while ((!recv_packet_args.setup_complete)) {
		;
	}
	sendto(sock, packet, packet_size, 0, (struct sockaddr *)arg.addr,
		   get_addr_len(arg.addr));

	pthread_join(thread_id, (void **)&recv_packet);

	if (recv_packet == NULL) {
		result->state = -1;

		// This needs the && because the first check is for if its empty or not
		// The second is only run when its not empty
	} else if (recv_packet->packet_header != NULL &&
			   recv_packet->packet_header->len > 0) {
		result->state = -1;

		const u_char *payload = (u_char *)recv_packet->packet_payload;

		// Skip Ethernet header (14 bytes)
		const u_char *ip_header = payload + 14;

		// Skip IP header
		const u_char *tcp_header = ip_header + ip_hdr_len;

		// cast to tcphdr
		struct tcphdr *recv_tcp = (struct tcphdr *)tcp_header;

		// Check SYN-ACK flags too see if the port is open or not
		if (recv_tcp->ack && recv_tcp->syn) {
			result->state = 1; // Port is open
		} else {
			result->state = 0; // Port is closed/filtered
		}

		free(recv_packet->packet_payload);
		free(recv_packet->packet_header);
		free(recv_packet);
	} else {
		// Did not reponde with either ACK or RST
		result->state = 0;
	}
	clock_gettime(CLOCK_MONOTONIC, &stop);
	result->scannig_time = time_in_x(start, stop, NANO);
	socket_close(sock);
	return 0;
}
