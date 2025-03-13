#pragma once

#include <netdb.h>
#include <stdlib.h>
#include <string.h>

#include "../../ui/print_macro.h"

#define MAX_DOMAIN_NAME 124

int fetch_domain(char domain[MAX_DOMAIN_NAME], struct sockaddr_storage *addr);
