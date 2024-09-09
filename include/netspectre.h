#ifndef NETSPECTRE_H
#define NETSPECTRE_H

#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <net/ethernet.h>
#include <netinet/ip.h>

#include <netinet/in.h>
#include <string.h>
#include <stdbool.h>
// https://www.gnu.org/software/libc/manual/html_node/Argp-Example-3.html
#include <argp.h>
#include <signal.h>
#include <sys/socket.h>
#include <netdb.h>
#include <pthread.h> 
#include <unistd.h>
#include <ifaddrs.h>
#include <netinet/ip_icmp.h>
#include <linux/if.h>

#include "other/outputs.h"
#include "other/parse_ports.h"
#include "other/ping.h"

#define IPV4_ADDR_STR_LEN 16 // 15 characters for the address and 1 for the null terminator

#define MAX_PACKET_SIZE 65663

typedef struct {
    struct pcap_pkthdr * packet_header;
    unsigned char * packet_payload;
} net_packet;

#endif