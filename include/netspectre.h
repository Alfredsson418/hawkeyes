#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>

// https://www.gnu.org/software/libc/manual/html_node/Argp-Example-3.html
#include <argp.h>


#include <pcap.h>
#include <ifaddrs.h>


#include <arpa/inet.h>

#include <net/ethernet.h>
#include <netinet/ip.h>

#include <netinet/in.h>



#include <sys/socket.h>
#include <netdb.h>

#include <sys/types.h>
#include <unistd.h>

#include <netinet/ip_icmp.h>
#include <linux/if.h>

#include "other/outputs.h"
#include "other/parse_ports.h"
#include "other/ping.h"
#include "other/read_csv.h"
#include "other/is_root.h"

#define IPV4_ADDR_STR_LEN 16 // 15 characters for the address and 1 for the null terminator
#define MAX_NETWORK_INTERFACE_LEN 32

#define MAX_PACKET_SIZE 65663

typedef struct {
    struct pcap_pkthdr * packet_header;
    u_char * packet_payload;
} net_packet;


// Used for scanning functions so that it is easier to call
// them as function pointers
typedef struct {
    struct in_addr ipv4;
    char * network_interface;
    int timeout;
    int port;
}scan_function_arguments;
