#pragma once

#include "../netspectre.h"

int find_port(scan_methods_t method, unsigned short port, char (*service)[PORT_SERVICE_LEN]);
