#include "../../include/other/ping.h"

// Sometimes this gets no responce, even though target is up
int ping(struct sockaddr_storage ip_addr, const char interface[INTERFACE_LEN]) {
	// Set a timeout for recvfrom
	struct timeval timeout;
	timeout.tv_sec			   = 5; // 5 seconds timeout
	timeout.tv_usec			   = 0;
	int				   echo_ID = 8765;
	int				   sock;
	int				   tries = 2;
	char			   buffer[1024];
	struct sockaddr_in sender_addr;
	socklen_t		   sender_addr_len = sizeof(sender_addr);
	struct sockaddr	   addr;
	ssize_t			   sent;

	struct icmphdr	 icmp;
	struct icmp6_hdr icmp6;

	memset(&addr, 0, sizeof(addr));
	if (ip_addr.ss_family == AF_INET) {

		// Setting up ICMP echo package
		memset(&icmp, 0, sizeof(icmp));
		icmp.type			  = ICMP_ECHO;
		icmp.un.echo.id		  = echo_ID;
		icmp.un.echo.sequence = htons(1);
		icmp.code			  = 0;
		icmp.checksum		  = checksum(&icmp, sizeof(icmp));

		sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

		struct sockaddr_in *addr_in = (struct sockaddr_in *)&addr;
		addr_in->sin_port			= htons(0);
		addr_in->sin_family			= AF_INET;
		addr_in->sin_addr = ((struct sockaddr_in *)&ip_addr)->sin_addr;

	} else if (ip_addr.ss_family == AF_INET6) {

		// Setting up ICMP6 echo package
		memset(&icmp6, 0, sizeof(icmp6));
		icmp6.icmp6_type = ICMP6_ECHO_REQUEST;
		// icmp6_hdr.icmp6_id = echo_ID; // Unique ID
		//  icmp6_hdr.icmp6_seq = htons(1);               // Sequence number
		icmp6.icmp6_dataun.icmp6_un_data16[0] = echo_ID;
		icmp6.icmp6_dataun.icmp6_un_data16[1] = htons(1);
		icmp6.icmp6_code					  = 0;
		icmp6.icmp6_cksum					  = checksum(&icmp6, sizeof(icmp6));

		sock = socket(AF_INET6, SOCK_RAW, IPPROTO_ICMPV6);

		struct sockaddr_in6 *addr_in = (struct sockaddr_in6 *)&addr;
		addr_in->sin6_port			 = htons(0);
		addr_in->sin6_family		 = ip_addr.ss_family;
		addr_in->sin6_addr = ((struct sockaddr_in6 *)&ip_addr)->sin6_addr;

	} else {
		ERR_PRINT("Wrong format\n");
		return -1;
	}

	if (sock < 0) {
		ERR_PRINT("Failed opening UDP socket for ping\n");
		return -2;
	}

	if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) <
		0) {
		ERR_PRINT("Failed to set a timeout to socket\n");
		close(sock);
		return -3;
	}

	// Bind the socket to the specified network interface
	if (setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, interface,
				   INTERFACE_LEN) < 0) {
		ERR_PRINT("Failed to bind to device %s\n", interface);
		close(sock);
		return -4;
	}

	// Pinging adress on interface
	if (ip_addr.ss_family == AF_INET) {
		sent = sendto(sock, &icmp, sizeof(icmp), 0, &addr, sizeof(addr));

	} else if (ip_addr.ss_family == AF_INET6) {
		sent = sendto(sock, &icmp6, sizeof(icmp6), 0, &addr,
					  sizeof(struct sockaddr_in6));

	} else {
		ERR_PRINT("Wrong format\n");
		close(sock);
		return -5;
	}

	if (sent < 0) {
		perror("sock");
		ERR_PRINT("Failed to send ICMP/ping packet \n");
		close(sock);
		return -6;
	}

	// This is needed because the first packet could be the package sent
	for (int i = 0; i < tries; i++) {
		// Recovering echo packet
		ssize_t received =
			recvfrom(sock, buffer, sizeof(buffer), 0,
					 (struct sockaddr *)&sender_addr, &sender_addr_len);
		if (received < 0) {
			close(sock);
			return -7;
		}

		if (ip_addr.ss_family == AF_INET) {
			struct iphdr   *ip_hdr = (struct iphdr *)buffer;
			struct icmphdr *recv_icmp_hdr =
				(struct icmphdr *)(buffer + (ip_hdr->ihl * 4));

			if (recv_icmp_hdr->type == ICMP_ECHOREPLY &&
				recv_icmp_hdr->un.echo.id == echo_ID) {
				close(sock);
				return 1;
			}
		} else if (ip_addr.ss_family == AF_INET6) {
			// Apperently, the ipv6 header is cut of frn recv, so there is no
			// need to move the buffer
			struct icmp6_hdr *icmp6_header = (struct icmp6_hdr *)(buffer);

			if (icmp6_header->icmp6_type == ICMP6_ECHO_REPLY &&
				icmp6_header->icmp6_dataun.icmp6_un_data16[0] == echo_ID) {
				close(sock);
				return 1;
			}
		}
	}

	close(sock);
	return 0;
}
