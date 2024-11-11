#include "../../include/scan/scan_main.h"


int scan(int argc, char *argv[]) {
    // Varibles for argp
    struct terminal_scan_arguments *arguments = malloc(sizeof(struct terminal_scan_arguments));

    if (arguments == NULL) {
        ERR_PRINT("%s\n", "Failed to allocate memory for arguments");
        exit(0);
    }

    arguments->device = NULL;
    arguments->ports_format = NULL;
    arguments->scan_protocol = 0;
    arguments->ports = NULL;
    arguments->timeout = 3;
    arguments->no_ping = false;
    arguments->no_threading = false;
    arguments->thread_workers = 3;

    // If this is one, we know that the network device was given as a parameter
    bool parameter_device = true;

    argp_parse(&terminal_scan_argp, argc, argv, 0, 0, arguments);
    VERBOSE_MESSAGE("%s\n", "-------OUTPUT SETTINGS-------");
    VERBOSE_MESSAGE("%s %d\n", "Verbose is set to:", g_verbose_enabled);
    VERBOSE_MESSAGE("%s %d\n", "Output is set to:", g_no_terminal_output);
    VERBOSE_MESSAGE("%s\n", "-------ARGUMENT SETTINGS-------");
    VERBOSE_MESSAGE("Target: %s\n", inet_ntoa(arguments->target));
    VERBOSE_MESSAGE("No Threading: %d\n", arguments->no_threading);
    VERBOSE_MESSAGE("No Ping before scanning: %d\n", arguments->no_threading);
    VERBOSE_MESSAGE("Thread Workers: %d\n", arguments->thread_workers);
    if (arguments->device == NULL) {
        VERBOSE_MESSAGE("%s\n", "No network device set");
        if (arguments->no_ping == false) {
            arguments->device = guess_ping(arguments->target);
        } else {
            VERBOSE_MESSAGE("%s\n", "Could not guess network device, using first");
            arguments->device = get_first_network_dev();
        }
        parameter_device = false;
    }
    VERBOSE_MESSAGE("Network Device: %s\n", arguments->device);
    if (arguments->ports_format == NULL) {
        arguments->ports_format = "1-1000";
        VERBOSE_MESSAGE("%s %s\n", "Port range was not set, using default", arguments->ports_format);
    }

    PRINT("%s %s\n", "Scanning on ports:", arguments->ports_format);
    arguments->ports_len = parse_ports(arguments->ports_format, &(arguments->ports));




    scan_function_arguments function_argument;
    function_argument.ipv4 = arguments->target;
    function_argument.network_interface = arguments->device;
    function_argument.timeout = arguments->timeout;
    // This will be where the scanning starts
    int * ports_result;
    int (*function)(scan_function_arguments);
    switch (arguments->scan_protocol) {
        case (0):
            function = tcp_scan;
            break;
        case (1):
            function = udp_scan;
            break;
        default:
            ERR_PRINT("%s\n", "An error occured when trying to determine scanning protocol");
            exit(0);
    }

    if (arguments->no_threading){
        ; // This will be implemented laterS
    } else {
        ports_result = multithread_scanning(arguments->thread_workers, arguments->ports, arguments->ports_len, function, function_argument);
    }

    for (int i = 0; i < arguments->ports_len; i++) {
        if (*(ports_result + i) == 1) {
            PRINT("%d is open\n", *(arguments->ports + i));
        }
    }

    if (!parameter_device) {
        free_dev(arguments->device);
    }
    PRINT("%s\n", "exit");
    free(ports_result);
    free(arguments->ports);
    free(arguments);
    return 0;
}
