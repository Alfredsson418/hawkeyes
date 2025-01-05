#include "../../include/other/network_interface.h"

int get_first_network_dev(char (*interface)[INTERFACE_LEN]) {
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
            continue;
        }
        if (strlen(ifaddr->ifa_name) > INTERFACE_LEN) {
            ERR_PRINT("Interface name too long\n");
            continue;
        }
        strncpy(*interface, ifaddr->ifa_name, strlen(ifaddr->ifa_name));
        return 1;
    }
    return -1;
}

int guess_interface(struct sockaddr_storage ip_addr,
                    char (*interface)[INTERFACE_LEN]) {

    // Get network interfaces
    struct ifaddrs *network_interfaces;
    if (getifaddrs(&network_interfaces) < 0) {
        ERR_PRINT("Failed to featch network interfaces\n");
        return -1;
    }

    // Start looping though interfaces to ping
    for (struct ifaddrs *ifa = network_interfaces; ifa != NULL;
         ifa                 = ifa->ifa_next) {
        // If the network interface has an IPv4 adress

        if (ifa->ifa_addr != NULL &&
            ifa->ifa_addr->sa_family == ip_addr.ss_family) {

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
                if (strlen(ifa->ifa_name) > INTERFACE_LEN) {
                    ERR_PRINT("Too long interface name\n");
                    freeifaddrs(network_interfaces);
                    return -1;
                }
                strncpy(*interface, ifa->ifa_name, strlen(ifa->ifa_name));
                freeifaddrs(network_interfaces);
                return 1;
            }
            VERBOSE_MESSAGE("FAILED\n");
        }
    }
    freeifaddrs(network_interfaces);
    return 0;
}

int verify_interface(char interface[INTERFACE_LEN]) {
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
        if (strcmp(interface, ifaddr->ifa_name) == 0) {
            return 1;
        }
    }
    return 0;
}
