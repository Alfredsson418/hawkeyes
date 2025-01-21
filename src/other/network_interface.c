#include "../../include/other/network_interface.h"

int _ping(struct ifaddrs *ifa, struct sockaddr_storage ip_addr) {
    int respone;
    VERBOSE_MESSAGE("PING REQUEST: Trying on '%s': ", ifa->ifa_name);
    for (int i = 1; i <= 2; i++) {
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
    // Copy address
    memcpy(&ip_addr->s_addr, ifa->ifa_addr,
           ifa->ifa_addr->sa_family == AF_INET ? sizeof(struct sockaddr_in)
                                               : sizeof(struct sockaddr_in6));

    // Copy subnet mask
    memcpy(&ip_addr->subnet_mask, ifa->ifa_netmask,
           ifa->ifa_netmask->sa_family == AF_INET
               ? sizeof(struct sockaddr_in)
               : sizeof(struct sockaddr_in6));

    // Copy address family
    ip_addr->s_addr.ss_family = ifa->ifa_addr->sa_family;

    // Copy name
    strncpy(ip_addr->name, ifa->ifa_name, strlen(ifa->ifa_name));
}

int get_first_network_interface(interface_info *interface) {
    struct ifaddrs *ifaddr;

    if (getifaddrs(&ifaddr) < 0) {
        ERR_PRINT("Failed to featch network interfaces\n");
        return -1;
    }

    for (; ifaddr != NULL; ifaddr = ifaddr->ifa_next) {
        if (ifaddr->ifa_addr == NULL) {
            continue;
        }
        if (strcmp(ifaddr->ifa_name, "lo") <= 0) {
            // HERE TO SEE IF TARGET IP IS IN RANGE OF SOURCE
            // IF THEN SET lo TO INTERFACE

            continue;
        }
        if (strlen(ifaddr->ifa_name) > INTERFACE_LEN) {
            ERR_PRINT("Interface name too long\n");
            continue;
        }

        _copy_addr(ifaddr, interface);

        return 1;
    }
    return -1;
}

int guess_interface(struct sockaddr_storage ip_addr,
                    interface_info         *interface) {

    // Get network interfaces
    struct ifaddrs *network_interfaces;
    if (getifaddrs(&network_interfaces) < 0) {
        ERR_PRINT("Failed to featch network interfaces\n");
        return -1;
    }

    // Start looping though interfaces to ping
    for (struct ifaddrs *ifa = network_interfaces; ifa != NULL;
         ifa                 = ifa->ifa_next) {

        if (ifa->ifa_addr != NULL &&
            ifa->ifa_addr->sa_family == ip_addr.ss_family) {
        }

        if (is_root() && _ping(ifa, ip_addr)) {
            _copy_addr(ifa, interface);
            break;
        }
    }
    freeifaddrs(network_interfaces);
    return 0;
}

int verify_interface(interface_info *interface) {
    struct ifaddrs *ifaddr;

    if (getifaddrs(&ifaddr) < 0) {
        ERR_PRINT("Failed to featch network interfaces\n");
        return -1;
    }

    for (; ifaddr != NULL; ifaddr = ifaddr->ifa_next) {
        if (ifaddr->ifa_addr == NULL) {
            continue;
        }
        if (strlen(ifaddr->ifa_name) > INTERFACE_LEN) {
            ERR_PRINT("Interface name too long\n");
            continue;
        }
        if (strcmp(interface->name, ifaddr->ifa_name) == 0) {
            return 1;
        }
    }
    return 0;
}
