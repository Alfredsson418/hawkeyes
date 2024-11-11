#pragma once

#include "../netspectre.h"

/*
    Parameters:
        str:  The string from where to count the amount of ports
    Return:
        Amount of ports.

*/
int count_ports(char * str);

/*
    Parameters:
        str:  The string from where to parse the ports from, can take - and ,
        ports:  An empty array of integers.
    Return:
        Length of array.

*/
int parse_ports(char * str, int ** ports);
