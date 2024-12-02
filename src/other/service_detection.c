#include "../../include/other/service_detection.h"


int find_port(scan_methods_t method, unsigned short port, char (*service)[PORT_SERVICE_LEN]) {
    FILE * file_ptr;
    switch (method) {
        case SCAN_TCP_t:
        case SCAN_HALF_SYNC_t:
            file_ptr = fopen(TCP_SERVICES_FILE, "r");
            break;
        case SCAN_UDP_t:
            file_ptr = fopen(UDP_SERVICES_FILE, "r");
            break;
        default:
            ERR_PRINT("No protocol file \n");
            return -1;
    }

    if (file_ptr == NULL) {
        ERR_PRINT("Error opening file \n");
        return -1;
    }

    char a[PORT_SERVICE_LEN];
    while (fgets(a, sizeof(a), file_ptr)){
        char * rest = a;
        char * csv_port = strtok_r(rest, ",", &rest);
        char * csv_service = strtok_r(rest, ",", &rest);
        int temp = atoi(csv_port);
        if (temp > MAX_PORT) {
            ERR_PRINT("Ports bigger that %d not allowed\n", MAX_PORT);
            return -1;
        } else if (temp < 0) {
            ERR_PRINT("Port to small\n");
            return -1;
        }
        unsigned short temp_port = temp;
        if (temp_port == port) {
            if (strlen(csv_service) > PORT_SERVICE_LEN) {
                ERR_PRINT("Service name is to long\n");
                fclose(file_ptr);
                return -1;
            }
            strncpy(*service, csv_service, PORT_SERVICE_LEN);

            // This removes last new_row
            (*service)[strlen(*service) - 1] = '\0';

            fclose(file_ptr);
            return 1;
        } else if (temp_port > port){
            return 0;
        }
    }

    fclose(file_ptr);
    return 0;
}
