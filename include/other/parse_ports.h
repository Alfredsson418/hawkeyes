#pragma once

#include <stdlib.h>
#include <string.h>

#include "../constans.h"
#include "../ui/print_macro.h"

/*
	Parameters:
		str:  The string from where to parse the ports from, can take - and ,
		ports:  An empty array of integers.
	Return:
		Length of array.

*/
unsigned int parse_ports(const char *str, unsigned short **ports);
