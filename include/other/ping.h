#ifndef PING_H
#define PING_H


#include <netinet/ip_icmp.h>
#include <linux/if.h>

#include "../netspectre.h"



/*
    Parameters:
        ip: The IP to ping
    Return:
        1 if reach, 0 if not
*/
int ping(struct in_addr ip_addr, const char * interface);

#endif