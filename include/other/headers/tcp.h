#pragma once

#include <stdlib.h>

// Pseudo header needed for TCP checksum calculation
typedef struct {
    u_int32_t source_address;
    u_int32_t dest_address;
    u_int8_t  placeholder;
    u_int8_t  protocol;
    u_int16_t tcp_length;
} tcp_pseudo_header;
