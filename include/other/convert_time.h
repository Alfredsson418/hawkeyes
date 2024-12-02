#pragma once


#include <time.h>
#include <stdlib.h>

#define SECONDS_S 1000000000
#define MILLI_S 1000000
#define MICRO_S 1000
#define NANO_S 1


double time_in_x(struct timespec start, struct timespec stop, int x);
