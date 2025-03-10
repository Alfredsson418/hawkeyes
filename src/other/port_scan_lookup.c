#include "../../include/other/port_scan_lookup.h"

int calculate_scanned(scan_result_t *a, int n) {
	double scanned = 0;
	for (int i = 0; i < n; i++) {
		if (a[i].state >= 0) {
			scanned++;
		}
	}
	return scanned;
}

char *state_string(int state) {
	switch (state) {
	case (0):
		return "Closed";
	case (1):
		return "Open";
	default:
		return "Err";
	}
}
