#include "../../include/scan/scan_main.h"


int scan(int argc, char *argv[]) {
    // Varibles for argp
    struct terminal_scan_arguments arguments;


    arguments.target.s_addr = INADDR_NONE;
    memset(arguments.interface, '\0', INTERFACE_LEN);
    memset(arguments.ports_format, '\0', PORTS_FORMAT_LEN);
    arguments.scan_protocol = 0;
    arguments.ports = NULL;
    arguments.timeout = 3;
    arguments.no_ping = false;
    arguments.thread_workers = 3;

    argp_parse(&terminal_scan_argp, argc, argv, 0, 0, &arguments);
    VERBOSE_MESSAGE("-------OUTPUT SETTINGS-------\n");
    VERBOSE_MESSAGE("Verbose is set to: %d\n", g_verbose_enabled);
    VERBOSE_MESSAGE("Output is set to: %d\n", g_no_terminal_output);
    VERBOSE_MESSAGE("-------ARGUMENT SETTINGS-------\n");
    VERBOSE_MESSAGE("Scanning Method: %d \n", arguments.scan_protocol);
    VERBOSE_MESSAGE("Target: %s \n", inet_ntoa(arguments.target));
    VERBOSE_MESSAGE("No Ping before scanning: %s \n", arguments.no_ping ? "True" : "False");
    VERBOSE_MESSAGE("Thread Workers: %d \n", arguments.thread_workers);

    if (arguments.target.s_addr == INADDR_NONE) {
        ERR_PRINT("No target specified\n");

        return 0;
    }

    if (arguments.interface[0] == '\0') {
        VERBOSE_MESSAGE("No network device set \n");

        if ((!arguments.no_ping && is_root()) && guess_interface(arguments.target, &(arguments.interface)) < 0) {
            ERR_PRINT("Failed to ping target, try using --no-ping\n");
        }
        else {
            get_first_network_dev(&(arguments.interface));
        }
    }
    VERBOSE_MESSAGE("Network interface: %s\n", arguments.interface);
    if (arguments.ports_format[0] == '\0') {
        strncpy(arguments.ports_format, "1-1000", INTERFACE_LEN);
        VERBOSE_MESSAGE("Port range was not set, using default %s \n", arguments.ports_format);
    }

    arguments.ports_len = parse_ports(arguments.ports_format, &(arguments.ports));
    if (arguments.ports_len < 1) {
        ERR_PRINT("Failed to parse ports\n");
        return -1;
    }
    PRINT("Scanning on ports: %s \n", arguments.ports_format);



    scan_function_arguments function_argument;
    function_argument.ipv4 = arguments.target;
    strcpy(function_argument.network_interface, arguments.interface);
    function_argument.timeout = arguments.timeout;
    // This will be where the scanning starts
    int * ports_result;
    int (*function)(scan_function_arguments);
    switch (arguments.scan_protocol) {
        case (TCP_NUM):
            function = tcp_scan;
            break;
        case (UDP_NUM):
            function = udp_scan;
            break;
        default:
            ERR_PRINT("An error occured when trying to determine scanning protocol\n");
            return -1;
    }


    ports_result = multithread_scanning(arguments.thread_workers, arguments.ports, arguments.ports_len, function, function_argument);


    PRINT("----Open Ports----\n");

    for (int i = 0; i < arguments.ports_len; i++) {
        if (ports_result[i] == 1) {
            PRINT(" -> %d/ ", arguments.ports[i]);
            char service[PORT_SERVICE_LEN];
            if (find_port(arguments.scan_protocol, arguments.ports[i], &service)) {
                PRINT("%s", service);
            }
            PRINT("\n");
        }
    }
    PRINT("------------------\n");

    free(ports_result);
    free(arguments.ports);
    return 0;
}
