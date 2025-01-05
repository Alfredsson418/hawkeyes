#pragma once

#include <stdlib.h>
#include <time.h>

#define SECONDS 1000000000
#define MILLI   1000000
#define MICRO   1000
#define NANO    1

double time_in_x(struct timespec start, struct timespec stop, int x);
