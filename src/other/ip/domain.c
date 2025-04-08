#include "../../../include/other/ip/domain.h"

int fetch_domain(char domain[MAX_DOMAIN_NAME], struct sockaddr_storage *addr) {
	memset(addr, 0, sizeof(*addr));
	char *af_family;

	char *new_context = strtok_r(domain, ":", &af_family);

	struct addrinfo	 hints;
	struct addrinfo *results, *rp;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family	   = AF_UNSPEC;	 /* Allow IPv4 or IPv6 */
	hints.ai_socktype  = SOCK_DGRAM; /* Datagram socket */
	hints.ai_flags	   = AI_PASSIVE; /* For wildcard IP address */
	hints.ai_protocol  = 0;			 /* Any protocol */
	hints.ai_canonname = NULL;
	hints.ai_addr	   = NULL;
	hints.ai_next	   = NULL;

	int res = getaddrinfo(new_context, NULL, &hints, &results);

	if (res != 0) {
		ERR_PRINT("%s\n", gai_strerror(res));
		return -1;
	}

	/*
		if af_family is empty
			- Populate first
			- If addr is IPv6 and IPv4 is next, overwrite
		If af_family is 4
			- If ipv4 does not exist, return -1;
		if af_family is 6
			- If ipv6 does not exist, return -1;
	*/

	if (strcmp(af_family, "4") != 0 && strcmp(af_family, "6") != 0) {
		af_family = "0";
	}

	for (rp = results; rp != NULL; rp = rp->ai_next) {
		/*
			This may look unnessesary but its not,
			This restricts that IPv4 dont overwrite
			IPv6 and vise versa.
		*/
		if (strcmp(af_family, "0") == 0 && addr->ss_family != AF_INET) {
			goto hit;
		} else if (strcmp(af_family, "4") == 0 && rp->ai_family == AF_INET) {
			goto hit;
		} else if ((strcmp(af_family, "6") == 0 && rp->ai_family == AF_INET6)) {
			goto hit;
		} else {
			continue;
		}
	hit:
		memcpy(addr, rp->ai_addr, rp->ai_addrlen);
	}

	freeaddrinfo(results);
	if (addr->ss_family == 0 || addr->ss_family == AF_UNSPEC) {
		return -2;
	}
	return 0;
}
