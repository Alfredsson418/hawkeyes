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

    // Create a new array to store the merged strings
    char *processed_argv[argc - 1];

    /*
        Merge the first and second arguments into the format "%s %s"
        The merge is nessesary because to show the help and usage command correctly for argp
    */
    char *merged_string = malloc(strlen(argv[0]) + strlen(argv[1]) + 2);
    if (merged_string == NULL) {
        ERR_PRINT("%s\n", "Error allocatin memory for merged string!");
        exit(0);
    }
    sprintf(merged_string, "%s %s", argv[0], argv[1]);

    // Store the merged string in the new array
    processed_argv[0] = merged_string;

    // Copy the remaining arguments to the new array
    for (int i = 2; i < argc; i++) {
        processed_argv[i - 1] = argv[i];
    }

    display_motd();

    VERBOSE_MESSAGE("Running %s\n", argv[1]);
    if (strcasecmp(argv[1], "capture") == 0) {
        capture(argc -1, processed_argv);
    }else if (strcasecmp(argv[1], "scan") == 0) {
        scan(argc -1, processed_argv);
    }else if (strcasecmp(argv[1], "test") == 0) {
        int * a;
        int len = parse_ports("50,51,52,53,54,55,56,57,58,59,60,61", &a);
        for (int i = 0; i < len; i++) {
            PRINT("%d\n", a[i]);
        }
        PRINT("%d\n", len);

    } else {
        ERR_PRINT("%s\n", "Did not recognice command! Exiting!");
        exit(0);
    }

    free(merged_string);

    return 0;

}
