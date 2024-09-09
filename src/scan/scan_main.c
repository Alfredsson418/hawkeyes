#include "../../include/scan/scan_main.h"


int scan(int argc, char *argv[]) {
    // Varibles for argp
    struct scan_arguments *arguments = malloc(sizeof(struct scan_arguments));

    if (arguments == NULL) {
        ERR_PRINT("%s\n", "Failed to allocate memory for arguments");
        exit(0);
    }

    arguments->device = NULL;
    arguments->ports_format = NULL;
    arguments->scan_protocol = NULL;
    arguments->target = NULL;
    arguments->ports = NULL;
    arguments->timeout = 10;

    argp_parse(&scan_argp, argc, argv, 0, 0, arguments);
    VERBOSE_MESSAGE("%s\n", "-------OUTPUT SETTINGS-------");
    VERBOSE_MESSAGE("%s %d\n", "Verbose is set to:", g_verbose_enabled);
    VERBOSE_MESSAGE("%s %d\n", "Output is set to:", g_no_terminal_output);
    VERBOSE_MESSAGE("%s\n", "-------ARGUMENT SETTINGS-------");

    if (arguments->target == NULL) {
        ERR_PRINT("%s\n", "No target found!");
        exit(0);
    }

    if (arguments->device == NULL) {
        arguments->device = get_first_network_dev();
        if (arguments->device == NULL) {
            ERR_PRINT("%s\n", "Found no network device");
            exit(0);
        } else {
            VERBOSE_MESSAGE("%s %s\n", "Device was not set, first found is", arguments->device);
        }
    }
    if (arguments->ports_format == NULL) {
        arguments->ports_format = "1-1000";
        VERBOSE_MESSAGE("%s %s\n", "Port range was not set, using default", arguments->ports_format);
        
    }
    PRINT("%s %s\n", "Scanning on ports:", arguments->ports_format);
    arguments->ports_len = parse_ports(arguments->ports_format, &(arguments->ports));

    if (arguments->scan_protocol == NULL) {
        arguments->scan_protocol = "TCP";
        VERBOSE_MESSAGE("%s %s\n", "Scanning protocol was given, using default", arguments->scan_protocol);
    }


    udp_scan("127.0.0.1", 630, 10);


    free_dev(arguments->device);
    free(arguments->ports);
    free(arguments);
    return 0;


}