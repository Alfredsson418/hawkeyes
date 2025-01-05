#pragma once

#define RESULT_PORT_LEN    10 // 65535 + /tcp or /udp or other
#define RESULT_SERVICE_LEN PORT_SERVICE_LEN
#define RESULT_METHOD_LEN  20
#define RESULT_TIME_LEN    6
#define RESULT_STATE_LEN   8

#define MAX_PORTS_TO_SHOW 10

// 15 characters for the address and 1 for the null terminator
#define IPV4_ADDR_STR_LEN 16
#define INTERFACE_LEN     16
#define PORTS_FORMAT_LEN  32
#define PORT_SERVICE_LEN  16
#define MAX_PORT          65535

#define MAX_PACKET_SIZE 65663

#define MOTD_FILE         "data/motd.txt"
#define MOTD_WIDTH        80
#define TCP_SERVICES_FILE "data/services/tcp.txt"
#define UDP_SERVICES_FILE "data/services/udp.txt"
