#include "../include/hawkeyes.h"


int main(int argc, char *argv[]) {
    if (argc < 2) {
        ERR_PRINT("%s\n", "No arguments detected! Exiting!");
        exit(0);
    }

    print_file(MOTD_FILE, MOTD_WIDTH);
    /*
        ===========================================================
        VARIBLES INITIATION
        ===========================================================
    */
    struct terminal_args arguments;
    arguments.target.s_addr = INADDR_NONE;
    memset(arguments.interface, '\0', INTERFACE_LEN);
    memset(arguments.ports_format, '\0', PORTS_FORMAT_LEN);
    arguments.scan_method = 0;
    arguments.ports = NULL;
    arguments.timeout = 3;
    arguments.no_ping = false;
    arguments.thread_workers = 3;

    unsigned short open_ports = 0;
    double total_time_scanned = 0;
    /*
        ===========================================================
    */

    argp_parse(&terminal_argp, argc, argv, 0, 0, &arguments);

    /*
        ===========================================================
        VERBOSE ARBUMENTS OUTPUT
        ===========================================================
    */
    if (g_verbose_enabled) {
        ui_line("ARGUMENTS", '-', TERMINAL_WIDTH);
    }
    VERBOSE_MESSAGE("Scanning Method: %d \n", arguments.scan_method);
    VERBOSE_MESSAGE("Target: %s \n", inet_ntoa(arguments.target));
    VERBOSE_MESSAGE("Can ping before scan(?): %s \n", (!arguments.no_ping && is_root()) ? "Yes" : "No");
    VERBOSE_MESSAGE("Thread Workers: %d \n", arguments.thread_workers);
    VERBOSE_MESSAGE("Timeout (s): %d \n", arguments.timeout);


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
    } else {
        if (!verify_interface(arguments.interface)) {
            ERR_PRINT("The given network interface does not exist\n");
            return -1;
        }
    }

    VERBOSE_MESSAGE("Network interface: %s\n", arguments.interface);
    if (arguments.ports_format[0] == '\0') {
        strncpy(arguments.ports_format, "1-1000", INTERFACE_LEN);
        VERBOSE_MESSAGE("Port range was not set, using default %s \n", arguments.ports_format);
    }
    /*
        ===========================================================
    */


    int temp = parse_ports(arguments.ports_format, &(arguments.ports));
    if (temp < 1) {
        ERR_PRINT("Failed to parse ports\n");
        return -1;
    }
    arguments.ports_len = temp;

    PRINT("Scanning on ports: %s \n", arguments.ports_format);

    scan_arg_t function_argument;
    scan_result_t scan_result[arguments.ports_len];
    memset(scan_result, -1, sizeof(scan_result));

    function_argument.ipv4 = arguments.target;
    strcpy(function_argument.network_interface, arguments.interface);
    function_argument.timeout = arguments.timeout;
    // This will be where the scanning starts
    int (*function)(scan_arg_t, scan_result_t *);
    switch (arguments.scan_method) {
        case (SCAN_TCP_t):
            function = tcp_scan;
            break;
        case (SCAN_UDP_t):
            if (!is_root()) {
                ERR_PRINT("UDP scan require privliged permissions to run\n");
                return -1;
            }
            function = udp_scan;
            break;
        default:
            ERR_PRINT("An error occured when trying to determine scanning protocol\n");
            return -1;
    }
    ui_line("Starting Scan", '=', TERMINAL_WIDTH);


    multithread_scanning(arguments.thread_workers, arguments.ports, arguments.ports_len, function, function_argument, scan_result);


    ui_line("| Ports |", '=', TERMINAL_WIDTH);
    PRINT("|%-*s", RESULT_PORT_LEN, "Port");
    PRINT("|%-*s", RESULT_STATE_LEN, "State");
    PRINT("|%-*s", RESULT_SERVICE_LEN, "Service");
    PRINT("|%-*s", RESULT_METHOD_LEN, "Scanning Method");
    PRINT("|%-*s", RESULT_TIME_LEN, "Responce Time (s)");
    PRINT("\n");
    // ui_line("", '~', TERMINAL_WIDTH);
    for (int i = 0; i < arguments.ports_len; i++) {
        if (scan_result[i].state > 0) {
            open_ports++;
        }

        total_time_scanned += scan_result[i].scannig_time;

        if (arguments.ports_len > MAX_PORTS_TO_SHOW && scan_result[i].state <= 0) {
            continue;
        }

        char port_buff[RESULT_PORT_LEN];
        char service_buff[PORT_SERVICE_LEN];
        memset(service_buff, 0, sizeof(service_buff));

        find_port(arguments.scan_method, arguments.ports[i], &service_buff);
        sprintf(port_buff, "%u/%s", arguments.ports[i], get_transfer_layer_by_scan(arguments.scan_method));
        PRINT("|%-*s", RESULT_PORT_LEN, port_buff);
        PRINT("|%-*s", RESULT_STATE_LEN, state_string(scan_result[i].state));
        PRINT("|%-*s", RESULT_SERVICE_LEN, service_buff);
        PRINT("|%-*s", RESULT_METHOD_LEN, scanning_method(arguments.scan_method, scan_result[i].method));
        PRINT("|%-*f", RESULT_TIME_LEN, scan_result[i].scannig_time / SECONDS);
        PRINT("\n");
    }

    ui_line(" | General Stats | ", '=', TERMINAL_WIDTH);
    PRINT("\tOpen Ports -> %-*d\n", RESULT_PORT_LEN, open_ports);
    PRINT("\tClosed Ports -> %-*d\n", RESULT_PORT_LEN, arguments.ports_len - open_ports);
    PRINT("\tTotal time scanned -> %-*f\n", RESULT_PORT_LEN, total_time_scanned / SECONDS);

    free(arguments.ports);
    return 0;
}
