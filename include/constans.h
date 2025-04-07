#pragma once

/*
	================================
		DEFAULT PARAMETER VALUES
	================================
*/

#define DEFAULT_TIMEOUT		   3
#define DEFAULT_NO_PING		   false
#define DEFAULT_THREAD_WORKERS 3
#define DEFAULT_PORT_INTERVAL  "1-1000"

/*
	================================
		DEFAULT PROGRAM VALUES
	================================
*/

#define RESULT_PORT_LEN	   10 // 65535 + /tcp or /udp or other
#define RESULT_SERVICE_LEN PORT_SERVICE_LEN
#define RESULT_METHOD_LEN  20
#define RESULT_TIME_LEN	   6
#define RESULT_STATE_LEN   8

#define MAX_PORTS_TO_SHOW 10

// 15 characters for the address and 1 for the null terminator
#define IPV4_ADDR_STR_LEN 16
#define INTERFACE_LEN	  16
#define PORTS_FORMAT_LEN  32
#define PORT_SERVICE_LEN  16
#define MAX_PORT		  65535

#define MAX_PACKET_SIZE 65663

#define MOTD_FILE		  "data/motd.txt"
#define VERSION_FILE	  "data/version/version.json"
#define MOTD_WIDTH		  80
#define TCP_SERVICES_FILE "/tmp/hawkeyes/service/tcp"
#define UDP_SERVICES_FILE "/tmp/hawkeyes/service/udp"

#define RAND_PORT_UPPER_LIMIT 65535
#define RAND_PORT_LOWER_LIMIT 49152

// Service parser and storage
#define TMP_STORAGE		"/tmp/hawkeyes"
#define SERVICES		"/etc/services"
#define SERVICE_STORAGE "/tmp/hawkeyes/service/"
#define SERVICE_CHECK	"/tmp/hawkeyes/service/done"
