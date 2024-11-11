#pragma once

#include "../netspectre.h"
/*
    Parameters:
        payload:    the address to start dumping from.
        len:     the number of bytes to dump.
        perLine: number of bytes on each output line.
    Return:
        Void

    Got from https://stackoverflow.com/questions/7775991/how-to-get-hexdump-of-a-structure-data
    This function is also modified
*/
void hexdump(const unsigned char * payload, const int len, int perLine);
