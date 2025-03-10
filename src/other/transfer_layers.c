#include "../../include/other/transfer_layers.h"

char *get_transfer_layer_string(transfer_protocol_t protocol) {
	switch (protocol) {
	case (TCP_t):
		return "tcp";
	case (UDP_t):
		return "udp";
	default:
		return "err";
	}
}
