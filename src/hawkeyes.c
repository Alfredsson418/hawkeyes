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
    memset(&arguments.address, 0, sizeof(struct sockaddr_storage));
    memset(arguments.interface, '\0', INTERFACE_LEN);
    memset(arguments.ports_format, '\0', PORTS_FORMAT_LEN);
    memset(&arguments.scan_info, 0, sizeof(scan_func_t));
    arguments.ports          = NULL;
    arguments.timeout        = 3;
    arguments.no_ping        = false;
    arguments.thread_workers = 3;

    unsigned short open_ports         = 0;
    double         total_time_scanned = 0;
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

    if (arguments.scan_info.name != 0) {
        VERBOSE_MESSAGE("Scanning method: %s \n", arguments.scan_info.name);
    } else {
        ERR_PRINT("Missing scanning method\n");
        return -1;
    }

    if (arguments.address.ss_family == AF_INET) {
        char str[INET_ADDRSTRLEN];
        ipv4_to_str(&arguments.address, str);
        VERBOSE_MESSAGE("Target (IPv4): %s \n", str);
    } else if (arguments.address.ss_family == AF_INET6) {
        char str[INET6_ADDRSTRLEN];
        ipv6_to_str(&arguments.address, str);
        VERBOSE_MESSAGE("Target (IPv6): %s \n", str);
    } else {
        ERR_PRINT("Wrong input format\n");
        return -1;
    }

    VERBOSE_MESSAGE("Can ping before scan(?): %s \n",
                    (!arguments.no_ping && is_root()) ? "Yes" : "No");
    VERBOSE_MESSAGE("Thread Workers: %d \n", arguments.thread_workers);
    VERBOSE_MESSAGE("Timeout (s): %d \n", arguments.timeout);

    if (arguments.interface[0] == '\0') {
        VERBOSE_MESSAGE("No network interface set \n");
        if ((!arguments.no_ping && is_root()) &&
            guess_interface(arguments.address, &(arguments.interface)) < 0) {
            ERR_PRINT("Failed to ping target, try using --no-ping\n");
        } else {
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
        VERBOSE_MESSAGE("Port range was not set, using default %s \n",
                        arguments.ports_format);
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

    scan_arg_t    function_argument;
    scan_result_t scan_result[arguments.ports_len];

    memset(scan_result, -1, sizeof(scan_result));
    function_argument.addr = &arguments.address;
    strcpy(function_argument.network_interface, arguments.interface);
    function_argument.timeout = arguments.timeout;
    // This will be where the scanning starts
    ui_line("Starting Scan", '=', TERMINAL_WIDTH);

    multithread_scanning(&arguments.scan_info, &function_argument, scan_result,
                         arguments.ports, arguments.ports_len,
                         arguments.thread_workers);

    ui_line("| Ports |", '=', TERMINAL_WIDTH);
    PRINT("|%-*s", RESULT_PORT_LEN, "Port");
    PRINT("|%-*s", RESULT_STATE_LEN, "State");
    PRINT("|%-*s", RESULT_SERVICE_LEN, "Service");
    PRINT("|%-*s", RESULT_TIME_LEN, "Responce Time (s)");
    PRINT("\n");
    // ui_line("", '~', TERMINAL_WIDTH);
    for (int i = 0; i < arguments.ports_len; i++) {
        if (scan_result[i].state > 0) {
            open_ports++;
        }

        total_time_scanned += scan_result[i].scannig_time;

        if (arguments.ports_len > MAX_PORTS_TO_SHOW &&
            scan_result[i].state <= 0) {
            continue;
        }

        char port_buff[RESULT_PORT_LEN];
        char service_buff[PORT_SERVICE_LEN];
        memset(service_buff, 0, sizeof(service_buff));

        find_port(arguments.scan_info.transfer_protocol, arguments.ports[i],
                  &service_buff);
        sprintf(
            port_buff, "%u/%s", arguments.ports[i],
            get_transfer_layer_string(arguments.scan_info.transfer_protocol));
        PRINT("|%-*s", RESULT_PORT_LEN, port_buff);
        PRINT("|%-*s", RESULT_STATE_LEN, state_string(scan_result[i].state));
        PRINT("|%-*s", RESULT_SERVICE_LEN, service_buff);
        PRINT("|%-*f", RESULT_TIME_LEN, scan_result[i].scannig_time / SECONDS);
        PRINT("\n");
    }

    ui_line(" | General Stats | ", '=', TERMINAL_WIDTH);
    PRINT("\tOpen Ports -> %-*d\n", RESULT_PORT_LEN, open_ports);
    PRINT("\tClosed Ports -> %-*d\n", RESULT_PORT_LEN,
          arguments.ports_len - open_ports);
    PRINT("\tTotal time scanned -> %-*f\n", RESULT_PORT_LEN,
          total_time_scanned / SECONDS);

    free(arguments.ports);
    return 0;
}
