#pragma once

#include "../scan_structs.h"

int calculate_scanned(scan_result_t * a, int n);

char * state_string(int state);

char * scanning_method(scan_methods_t scan_method, int method);
