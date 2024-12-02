#include "../../include/other/parse_ports.h"

int parse_ports(char* str, unsigned short ** ports) {
    int len = 0;

    // Return array
    *ports = calloc(len, sizeof(int));
    char * token;
    int port_index = 0;

    char* str_copy = calloc(strlen(str), sizeof(char));
    if (str_copy == NULL) {
        ERR_PRINT("Failed to allocate memory\n");
        return -1;
    }
    strcpy(str_copy, str);

    char * save_ptr = str_copy;

    // parse port_str to ports array
    while ((token = strtok_r(str_copy, ",", &str_copy))) {
        if (strstr(token, "-") != NULL) {
            if (strlen(token) <= 1) {
                ERR_PRINT("Bad port input\n");
                free(save_ptr);
                free(*ports);
                return -1;
            }

            char * temp = strtok_r(token, "-", &token);
            if (temp == NULL) {
                ERR_PRINT("Bad port input\n");
                free(save_ptr);
                free(*ports);
                return -1;
            }
            int first = atoi(temp);
            temp = strtok_r(token, "-", &token);
            if (temp == NULL) {
                ERR_PRINT("Bad port input\n");
                free(save_ptr);
                free(*ports);
                return -1;
            }

            int second = atoi(temp);

            if (second < first) {
                ERR_PRINT("Bad port input\n");
                free(save_ptr);
                free(*ports);
                return -1;
            }

            if (first > MAX_PORT || second > MAX_PORT) {
                ERR_PRINT("Ports larger than %d does not exist\n", MAX_PORT);
                free(save_ptr);
                free(*ports);
                return -1;
            }

            // Count the amount of ports
            len += second - first + 1;

            *ports = realloc(*ports, len * sizeof(int));

            for (; first <= second; first++) {

                (*ports)[port_index] = first;
                port_index++;
            }
        } else {
            len++;
            *ports = realloc(*ports, len * sizeof(int));
            (*ports)[port_index] = atoi(token);
            port_index++;
        }
    }
    free(save_ptr);
    return len;
}
