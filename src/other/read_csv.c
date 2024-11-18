#include "../../include/other/read_csv.h"
#include <stdio.h>


int find_port(int protocol, int port, char * service) {
    FILE * file_ptr;
    switch (protocol) {
        case 2:
        case 0:
            file_ptr = fopen("data/services/tcp_services.txt", "r");
            break;
        case 1:
            file_ptr = fopen("data/services/udp_services.txt", "r");
            break;
        default:
            ERR_PRINT("No protocol file \n");
            return -1;
    }

    if (file_ptr == NULL) {
        ERR_PRINT("Error opening file \n");
        return -1;
    }

    char a[32];
    while (fgets(a, sizeof(a), file_ptr)){
        char * rest = a;
        char * csv_port = strtok_r(rest, ",", &rest);
        char * csv_service = strtok_r(rest, ",", &rest);
        if (atoi(csv_port) == port) {
            strcpy(service, csv_service);
            fclose(file_ptr);
            return 1;
        }
    }

    fclose(file_ptr);
    return 0;
}
