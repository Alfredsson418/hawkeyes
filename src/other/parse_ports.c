#include "../../include/other/parse_ports.h"

/*
    These functions works, but could be optimised
*/

int count_ports(char * str) {
    int len = 0;
    char * token;
    char* port_str = calloc(strlen(str) + 1, 1);
    strcpy(port_str, str);

    char * save_ptr = port_str;

    // counts ports
    while ((token = strtok_r(port_str, ",", &port_str))) {
        if (strstr(token, "-") != NULL) {
            int first = atoi(strtok_r(token, "-", &token));
            int second = atoi(strtok_r(token, "-", &token));
            if (second < first) {
                ERR_PRINT("%s\n", "Could not parse ports");
            }
            len += second - first + 1;
        } else {
            len++;
        }
    }
    free(save_ptr); // Free the allocated memory for port_str
    return len;
}

int parse_ports(char* str, int** ports) {
    int len = count_ports(str);
    *ports = calloc(len, sizeof(int));
    char * token;
    int port_i = 0;
    
    char* str_copy = calloc(strlen(str) + 1, sizeof(char));
    strcpy(str_copy, str);

    char * save_ptr = str_copy;

    // parse port_str to ports array
    while ((token = strtok_r(str_copy, ",", &str_copy))) {
        if (strstr(token, "-") != NULL) {
            int first = atoi(strtok_r(token, "-", &token));
            int second = atoi(strtok_r(token, "-", &token));

            if (second < first) {
                ERR_PRINT("%s\n", "Could not parse ports");
            }
            for (; first <= second; first++) {

                (*ports)[port_i] = first;
                port_i++;
            }
        } else {
            (*ports)[port_i] = atoi(token);
            port_i++;
        }
    }
    free(save_ptr);
    return len;
}
