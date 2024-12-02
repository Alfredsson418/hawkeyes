#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>


// https://www.gnu.org/software/libc/manual/html_node/Argp-Example-3.html
#include <argp.h>


#include <pcap.h>

#include <sys/socket.h>


#include <arpa/inet.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <linux/if.h>


typedef enum {
    SCAN_TCP_t = 0,
    SCAN_UDP_t = 1,
    SCAN_HALF_SYNC_t = 2,
}scan_methods_t;


#define MAX_PORTS_TO_SHOW 10


#define IPV4_ADDR_STR_LEN 16 // 15 characters for the address and 1 for the null terminator
#define INTERFACE_LEN 16
#define PORTS_FORMAT_LEN 32
#define PORT_SERVICE_LEN 16
#define MAX_PORT 65535

#define MAX_PACKET_SIZE 65663

#define MOTD_FILE "data/motd.txt"
#define MOTD_WIDTH 80
#define TCP_SERVICES_FILE "data/services/tcp.txt"
#define UDP_SERVICES_FILE "data/services/udp.txt"

#include "ui/print_macro.h"
#include "other/parse_ports.h"
#include "other/ping.h"
#include "other/is_root.h"
#include "ui/display_file.h"
#include "ui/terminal_display.h"


typedef struct {
    struct pcap_pkthdr * packet_header;
    u_char * packet_payload;
} net_packet;

typedef struct {
    int state;
    int method;
    double scannig_time; // Scanning time for port
}scan_result_t;

// Used for scanning functions so that it is easier to call
// them as function pointers
typedef struct {
    struct in_addr ipv4;
    char network_interface[INTERFACE_LEN];
    unsigned int timeout;
    unsigned short port;
}scan_arg_t;
