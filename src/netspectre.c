#include "../include/netspectre.h"
#include "../include/capture/capture_main.h"
#include "../include/scan/scan_main.h"

/*
    This is the main file where all the "outgoing commands" are sent like scan and capture
*/


int main(int argc, char *argv[]) {
    // Always 1 or more becase name of file is first
    if (argc < 2) {
        ERR_PRINT("%s\n", "No arguments detected! Exiting!");
        exit(0);
    }

    print_file(MOTD_FILE, MOTD_WIDTH);

    if (strcasecmp(argv[1], "capture") == 0) {
        capture(argc -1, argv + 1);
    }else if (strcasecmp(argv[1], "scan") == 0) {
        scan(argc -1, argv + 1);
    }else if (strcasecmp(argv[1], "test") == 0) {
        ;
    } else {
        ERR_PRINT("%s\n", "Did not recognice command! Exiting!");
        exit(0);
    }

    return 0;

}
