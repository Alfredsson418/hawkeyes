#pragma once

#include "../netspectre.h"

int find_port(int protocol, int port, char (*service)[PORT_SERVICE_LEN]);

void display_motd();
