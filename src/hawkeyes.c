#include "../include/hawkeyes.h"

int main(int argc, char *argv[]) {

	print_motd();
	PRINT("Version: ");
	print_json(VERSION_FILE, TERMINAL_WIDTH, "version");

	if (argc < 2) {
		ERR_PRINT("%s\n", "No arguments detected! Exiting!\n");
		exit(0);
	}
	/*
		===========================================================
		VARIBLES INITIATION
		===========================================================
	*/

	struct terminal_args arguments;
	interface_info		 interface;
	memset(&arguments.address, 0, sizeof(struct sockaddr_storage));
	memset(arguments.interface, '\0', INTERFACE_LEN);
	memset(arguments.ports_format, '\0', PORTS_FORMAT_LEN);
	memset(&arguments.scan_info, 0, sizeof(scan_func_t));
	arguments.timeout		 = DEFAULT_TIMEOUT;
	arguments.no_ping		 = DEFAULT_NO_PING;
	arguments.thread_workers = DEFAULT_THREAD_WORKERS;
	strncpy(arguments.ports_format, DEFAULT_PORT_INTERVAL, PORTS_FORMAT_LEN);
	arguments.force_parse_service_file = DEFAULT_FORCE_PARSE;

	unsigned short closed_ports		  = 0;
	unsigned short error_ports		  = 0;
	double		   total_time_scanned = 0;
	/*
		===========================================================
	*/

	argp_parse(&terminal_argp, argc, argv, 0, 0, &arguments);

	if (setup_services(arguments.force_parse_service_file) == 1) {
		VERBOSE_MESSAGE("Services already parsed\n");
	}

	interface.s_addr.ss_family = arguments.address.ss_family;

	/*
		===========================================================
		VERBOSE ARBUMENTS OUTPUT
		===========================================================
	*/
	if (g_verbose_enabled) {
		ui_line("ARGUMENTS", '-', TERMINAL_WIDTH);
	}

	if (arguments.scan_info.scan_func == NULL) {
		ERR_PRINT("No scanning method selected!\n");
		exit(0);
	}

	VERBOSE_MESSAGE("Scanning method: %s \n", arguments.scan_info.name);

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
		exit(0);
	}

	VERBOSE_MESSAGE("Can ping before scan(?): %s \n",
					(!arguments.no_ping && is_root()) ? "Yes" : "No");
	VERBOSE_MESSAGE("Thread Workers: %d \n", arguments.thread_workers);
	VERBOSE_MESSAGE("Timeout (s): %d \n", arguments.timeout);

	// Ugly ass spagetti code!!!!
	if (arguments.interface[0] == '\0') {
		VERBOSE_MESSAGE("No network interface set \n");

		// If root and want to ping
		if (!arguments.no_ping && is_root()) {
			VERBOSE_MESSAGE(" -> Guessing interface by pinging target! \n");

			if (guess_interface(arguments.address, &interface) < 0) {
				ERR_PRINT("Failed to ping target, using the --no-ping flag "
						  "will skip this step!\n");
			} else {
				goto interface_end;
			}
		}
		VERBOSE_MESSAGE(" -> Using first interface, please specify interface "
						"if its wrong \n");

		get_first_network_interface(&interface);

	} else { // If the user passes a interface
		strcpy(interface.name, arguments.interface);
		if (!verify_interface(&interface)) {
			ERR_PRINT("The given network interface does not exist\n");
			exit(0);
		}
	}

interface_end:

	VERBOSE_MESSAGE("Network interface: %s\n", interface.name);

	/*
		===========================================================
	*/
	unsigned short *ports;
	unsigned int	port_len = parse_ports(arguments.ports_format, &ports);
	if (port_len == 0) {
		ERR_PRINT("Failed to parse ports\n");
		exit(0);
	}

	PRINT("Scanning on ports: %s (%d)\n", arguments.ports_format, port_len);

	scan_arg_t	  function_argument;
	scan_result_t scan_result[port_len];

	memset(scan_result, -1, sizeof(scan_result));
	function_argument.addr		= &arguments.address;
	function_argument.interface = &interface;
	function_argument.timeout	= arguments.timeout;
	// This will be where the scanning starts
	ui_line("Starting Scan", '=', TERMINAL_WIDTH);

	multithread_scanning(&arguments.scan_info, &function_argument, scan_result,
						 ports, port_len, arguments.thread_workers);

	ui_line("| Ports |", '=', TERMINAL_WIDTH);
	PRINT("|%-*s", RESULT_PORT_LEN, "Port");
	PRINT("|%-*s", RESULT_STATE_LEN, "State");
	PRINT("|%-*s", RESULT_SERVICE_LEN, "Service");
	PRINT("|%-*s", RESULT_TIME_LEN, "Responce Time (s)");
	PRINT("\n");
	// ui_line("", '~', TERMINAL_WIDTH);
	for (int i = 0; i < port_len; i++) {
		if (scan_result[i].state == 0) {
			closed_ports++;
		}
		if (scan_result[i].state < 0) {
			error_ports++;
		}

		total_time_scanned += scan_result[i].scannig_time;

		if (port_len > MAX_PORTS_TO_SHOW && scan_result[i].state <= 0) {
			continue;
		}

		char port_buff[RESULT_PORT_LEN];
		char service_buff[PORT_SERVICE_LEN];
		memset(service_buff, 0, sizeof(service_buff));

		find_port(arguments.scan_info.transfer_protocol, ports[i],
				  &service_buff);
		sprintf(
			port_buff, "%u/%s", ports[i],
			get_transfer_layer_string(arguments.scan_info.transfer_protocol));
		PRINT("|%-*s", RESULT_PORT_LEN, port_buff);
		PRINT("|%-*s", RESULT_STATE_LEN, state_string(scan_result[i].state));
		PRINT("|%-*s", RESULT_SERVICE_LEN, service_buff);
		PRINT("|%-*f", RESULT_TIME_LEN, scan_result[i].scannig_time / SECONDS);
		PRINT("\n");
	}

	ui_line(" | General Stats | ", '=', TERMINAL_WIDTH);
	PRINT("\tOpen Ports -> %-*d\n", RESULT_PORT_LEN,
		  port_len - closed_ports - error_ports);
	PRINT("\tClosed Ports -> %-*d\n", RESULT_PORT_LEN, closed_ports);
	PRINT("\tError Ports -> %-*d\n", RESULT_PORT_LEN, error_ports);
	PRINT("\tTotal time scanned -> %-*f\n", RESULT_PORT_LEN,
		  total_time_scanned / SECONDS);

	free(ports);
	return 0;
}
