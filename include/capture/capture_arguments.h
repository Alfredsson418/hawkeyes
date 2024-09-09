#ifndef CAPTURE_ARGUMENTS_H
#define CAPTURE_ARGUMENTS_H

#include "../../include/netspectre.h"
#include "../other/network_device.h"

extern const struct argp_option capture_options[];

struct capture_arguments {
    char * format;
    char * device;
    int hexdump;
    char * pcap_load;
    char * filter;
    int capture_amount;
};
/*
    error_t capture_parse_opt(int key, char *arg, struct argp_state *state);
    
    Used as an callback function in argp_parse
*/
error_t capture_parse_opt(int key, char *arg, struct argp_state *state);

extern struct argp capture_argp;

#endif