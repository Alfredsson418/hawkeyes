#include "../../../include/other/ip/convert.h"
#include <netinet/in.h>

int str_to_ipv4(struct sockaddr_storage *addr, char str[INET_ADDRSTRLEN]) {
    struct sockaddr_in *addr_in = (struct sockaddr_in *)addr;
    addr_in->sin_family         = AF_INET;

    if (inet_pton(AF_INET, str, &(addr_in->sin_addr)) != 1) {
        return -1;
    }

    return 0;
}

int ipv4_to_str(struct sockaddr_storage *addr, char str[INET_ADDRSTRLEN]) {
    if (addr->ss_family != AF_INET) {
        ERR_PRINT("The provided sockaddr_storage does not contain an IPv4 "
                  "address.\n");
        return -1;
    }

    const struct sockaddr_in *addr_in = (const struct sockaddr_in *)addr;

    if (inet_ntop(AF_INET, &(addr_in->sin_addr), str, INET_ADDRSTRLEN) ==
        NULL) {
        return -1;
    }

    return 0;
}
int str_to_ipv6(struct sockaddr_storage *addr, char str[INET6_ADDRSTRLEN]) {
    struct sockaddr_in6 *addr_in = (struct sockaddr_in6 *)addr;
    addr_in->sin6_family         = AF_INET6;

    if (inet_pton(AF_INET6, str, &(addr_in->sin6_addr)) != 1) {
        return -1;
    }

    return 0;
}

int ipv6_to_str(struct sockaddr_storage *addr, char str[INET6_ADDRSTRLEN]) {
    if (addr->ss_family != AF_INET6) {
        ERR_PRINT("The provided sockaddr_storage does not contain an IPv4 "
                  "address.\n");
        return -1;
    }

    const struct sockaddr_in6 *addr_in = (const struct sockaddr_in6 *)addr;

    if (inet_ntop(AF_INET6, &(addr_in->sin6_addr), str, INET6_ADDRSTRLEN) ==
        NULL) {
        return -1;
    }

    return 0;
}
