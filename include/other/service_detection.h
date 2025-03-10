#pragma once

#include <stdio.h>
#include <stdlib.h> // atoi
#include <string.h>

#include "../constans.h"
#include "../ui/print_macro.h"
#include "transfer_layers.h"

int find_port(transfer_protocol_t method, unsigned short port,
			  char (*service)[PORT_SERVICE_LEN]);
