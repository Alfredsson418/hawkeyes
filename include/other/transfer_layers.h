#pragma once

// Could possibly add more in the future
// https://en.wikipedia.org/wiki/Transport_layer

typedef enum {
	TCP_t,
	UDP_t,
} transfer_protocol_t;

char *get_transfer_layer_string(transfer_protocol_t protocol);
