#include "../../include/other/network_device.h"

char * get_first_network_dev() {
    pcap_if_t *all_devices = NULL;
    char errbuf[PCAP_ERRBUF_SIZE];
    if (pcap_findalldevs(&all_devices, errbuf) == -1) {
        ERR_PRINT("%s\n", "Error finding devices");
        DEBUG_MESSAGE("%s\n", "pcap_findalldevs failed");
        exit(0);
    }
    
    if (all_devices == NULL) {
        ERR_PRINT("%s\n", "No network devices found");
        exit(0);
    }

    char *dev_string = NULL;
    if (all_devices->name != NULL) {
        dev_string = calloc(strlen(all_devices->name) + 1, sizeof(char));
        if (dev_string == NULL) {
            ERR_PRINT("%s\n", "Memory allocation failed");
            exit(0);
        }
        strncpy(dev_string, all_devices->name, strlen(all_devices->name));
    }

    pcap_freealldevs(all_devices);

    return dev_string;
}

// Kolla på hur den funkar
char* get_net_dev_by_ip(char target_ip[IPV4_ADDR_STR_LEN]) {
    struct ifaddrs *ifaddr, *ifa;
    char *device = NULL;

    // Get list of network interfaces
    if (getifaddrs(&ifaddr) == -1) {
        ERR_PRINT("%s\n", "ifaddrs");
        return NULL;
    }

    // Iterate through the list of interfaces
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET) {
            // Convert interface address to string
            char ifa_ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &((struct sockaddr_in*)ifa->ifa_addr)->sin_addr, ifa_ip, INET_ADDRSTRLEN);

            // Check if the target IP is reachable through this interface
            // This is a simplified check; you might need to compare subnet masks for a precise match
            if (strcmp(ifa_ip, target_ip) == 0) {
                device = calloc(strlen(ifa->ifa_name) + 1, sizeof(char));
                if (device == NULL) {
                    ERR_PRINT("%s\n", "Memory allocation failed");
                    exit(0);
                }
                strcpy(device, ifa->ifa_name);
                break;
            }
        }
    }

    freeifaddrs(ifaddr);
    return device; // Caller must free this memory
}

void free_dev(char * mem) {
    free(mem);
}