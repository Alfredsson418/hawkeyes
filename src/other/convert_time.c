#include "../../include/other/convert_time.h"

double time_in_x(struct timespec start, struct timespec stop, int x) {
	int64_t start_ns = start.tv_sec * SECONDS + start.tv_nsec;
	int64_t stop_ns	 = stop.tv_sec * SECONDS + stop.tv_nsec;

	return (double)(stop_ns - start_ns) / x;
}
