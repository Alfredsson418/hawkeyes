#pragma once

#include "../netspectre.h"

/*
    Parameters:
        str:  Double pointer to the string that is going to be processed
        old_sub:  What to replace
        new_sub:  What to replace with
    Return:
        Formated string
*/
void replace_substring(char **str, const char *old_sub, const char *new_sub);

/*
    Parameters:
        format:  Double pointer to the string that is going to be processed
        substring:  What to replace
        replacement:  What to replace with
    Return:
        Formated string
*/
void replace_format(char **format, char *substring, char *replacement);
