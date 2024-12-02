#pragma once

#include "../netspectre.h"

/*
    Parameters:
        str:  The string from where to parse the ports from, can take - and ,
        ports:  An empty array of integers.
    Return:
        Length of array.

*/
int parse_ports(char * str, unsigned short ** ports);
