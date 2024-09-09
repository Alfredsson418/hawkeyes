#ifndef OUTPUTS_H
#define OUTPUTS_H

#include "../netspectre.h"

extern int g_verbose_enabled;
extern int g_debug_enabled;
extern int g_no_terminal_output;

#define VERBOSE_MESSAGE(fmt, ...)\
do{\
    if(g_verbose_enabled && !g_no_terminal_output) fprintf(stdout, fmt, __VA_ARGS__);\
}while(0)

#define PRINT(fmt, ...)\
do{\
    if (!g_no_terminal_output) {\
        fprintf(stdout, fmt, __VA_ARGS__);\
    }\
}while(0)

#define ERR_PRINT(fmt, ...)\
do{\
    fprintf(stderr, "ERR: ");\
    fprintf(stderr, fmt, __VA_ARGS__);\
}while(0)

#endif