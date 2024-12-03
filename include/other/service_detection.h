#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h> // atoi

#include "../scan_structs.h"
#include "../ui/print_macro.h"
#include "../constans.h"

int find_port(scan_methods_t method, unsigned short port, char (*service)[PORT_SERVICE_LEN]);
