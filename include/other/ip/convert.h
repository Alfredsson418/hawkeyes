#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "../../ui/print_macro.h"

int str_to_ipv4(struct sockaddr_storage *addr, char str[INET_ADDRSTRLEN]);

int ipv4_to_str(struct sockaddr_storage *addr, char str[INET_ADDRSTRLEN]);

int str_to_ipv6(struct sockaddr_storage *addr, char str[INET6_ADDRSTRLEN]);

int ipv6_to_str(struct sockaddr_storage *addr, char str[INET6_ADDRSTRLEN]);
