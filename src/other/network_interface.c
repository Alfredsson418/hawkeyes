#include "../../include/other/network_interface.h"

int _ping(struct ifaddrs *ifa, struct sockaddr_storage ip_addr) {
	int tries = 2;
	int respone;
	VERBOSE_MESSAGE("PING REQUEST: Trying on '%s': ", ifa->ifa_name);
	for (int i = 0; i < tries; i++) {
		respone = ping(ip_addr, ifa->ifa_name);
		if (respone > 0) {
			VERBOSE_MESSAGE("SUCCESSFULL\n");
			break;
		}
	}
	if (respone == true) {
		return 1;
	}
	VERBOSE_MESSAGE("FAILED\n");
	return 0;
}

void _copy_addr(struct ifaddrs *ifa, interface_info *ip_addr) {
	if (ifa == NULL || ifa->ifa_addr == NULL) {
		// Handle error: invalid input
		return;
	}
	// Copy src addr
	memcpy(&ip_addr->s_addr, ifa->ifa_addr,
		   ip_addr->s_addr.ss_family == AF_INET ? sizeof(struct sockaddr_in)
												: sizeof(struct sockaddr_in6));

	// Copy subnet mask
	memcpy(&ip_addr->subnet_mask, ifa->ifa_netmask,
		   ip_addr->s_addr.ss_family == AF_INET ? sizeof(struct sockaddr_in)
												: sizeof(struct sockaddr_in6));

	// Copy name
	strcpy(ip_addr->name, ifa->ifa_name);
}

int get_first_network_interface(interface_info *interface) {
	struct ifaddrs *ifaddr;

	if (getifaddrs(&ifaddr) < 0) {
		ERR_PRINT("An error occured then trying to fetch network interfaces\n");
		return -1;
	}

	for (struct ifaddrs *i = ifaddr; i != NULL; i = i->ifa_next) {
		if (i->ifa_addr == NULL) {
			continue;
		}

		if (interface->s_addr.ss_family != i->ifa_addr->sa_family) {
			continue;
		}

		if (strlen(i->ifa_name) > INTERFACE_LEN) {
			ERR_PRINT("Interface name too long\n");
			continue;
		}

		if (strlen(i->ifa_name) > INTERFACE_LEN) {
			ERR_PRINT("Interface name too long\n");
			continue;
		}
		_copy_addr(i, interface);
		break;
	}
	freeifaddrs(ifaddr);
	return 0;
}

int guess_interface(struct sockaddr_storage ip_addr,
					interface_info		   *interface) {

	// Get network interfaces
	struct ifaddrs *ifaddr;

	if (getifaddrs(&ifaddr) < 0) {
		ERR_PRINT("An error occured then trying to fetch network interfaces\n");
		return -1;
	}

	// Start looping though interfaces to ping
	for (struct ifaddrs *i = ifaddr; i != NULL; i = i->ifa_next) {

		if (i->ifa_addr == NULL) {
			continue;
		}

		if (interface->s_addr.ss_family != i->ifa_addr->sa_family) {
			continue;
		}

		if (strlen(i->ifa_name) > INTERFACE_LEN) {
			ERR_PRINT("Interface name too long\n");
			continue;
		}

		if (is_root() && _ping(i, ip_addr)) {
			_copy_addr(i, interface);
			break;
		}
	}
	freeifaddrs(ifaddr);
	return 0;
}

int verify_interface(interface_info *interface) {
	struct ifaddrs *ifaddr;

	if (getifaddrs(&ifaddr) < 0) {
		ERR_PRINT("An error occured then trying to fetch network interfaces\n");
		return -1;
	}

	for (struct ifaddrs *i = ifaddr; i != NULL; i = i->ifa_next) {
		if (i->ifa_addr == NULL) {
			continue;
		}

		if (interface->s_addr.ss_family != i->ifa_addr->sa_family) {
			continue;
		}

		if (strlen(i->ifa_name) > INTERFACE_LEN) {
			ERR_PRINT("Interface name too long\n");
			continue;
		}
		if (strcmp(interface->name, i->ifa_name) == 0) {
			_copy_addr(i, interface);
			freeifaddrs(ifaddr);
			return 1;
		}
	}
	freeifaddrs(ifaddr);
	return 0;
}
