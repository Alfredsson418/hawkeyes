#include "../../include/other/read_data.h"


int find_port(int protocol, int port, char (*service)[PORT_SERVICE_LEN]) {
    FILE * file_ptr;
    switch (protocol) {
        case 2:
        case 0:
            file_ptr = fopen(TCP_SERVICES_FILE, "r");
            break;
        case 1:
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
        if (atoi(csv_port) == port) {
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
        }
    }

    fclose(file_ptr);
    return 0;
}


void display_motd() {
    FILE *file_ptr = fopen(MOTD_FILE, "r");
    if (file_ptr == NULL) {
        ERR_PRINT("Error opening MOTD file");
        return;
    }

    char buffer[MOTD_WIDTH];
    while (fgets(buffer, MOTD_WIDTH, file_ptr)) {
        PRINT("%s", buffer);
    }
    PRINT("\n");

    fclose(file_ptr);
}
