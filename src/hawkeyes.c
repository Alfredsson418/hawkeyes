#include "../include/hawkeyes.h"

struct arg_lit *arg_help, *arg_version, *arg_verbose, *arg_no_ping,
	*arg_parse_services;
struct arg_str *arg_method, *arg_interface, *arg_ports_string, *arg_target;
struct arg_int *arg_timeout, *arg_threads;
struct arg_end *end;

int main(int argc, char *argv[]) {

	print_motd();

	/*
		===========================================================
		Argument Parsing
		===========================================================
	*/

	char progname[] = "hawk";

	int			   nerrors;
	unsigned short exitcode = 0;

	void *argtable[] = {
		arg_help = arg_lit0("h", "help", "display this help and exit"),
		arg_version =
			arg_lit0(NULL, "version", "display version info and exit"),
		arg_verbose = arg_lit0("v", "verbose", "verbose output"),
		arg_no_ping = arg_lit0(NULL, "no-ping", "Do not ping the target"),
		arg_parse_services =
			arg_lit0(NULL, "parse-services",
					 "Force to reparse /etc/services to use in program"),
		arg_method =
			arg_str1("m", "method", "<METHOD>",
					 "The type of scanning method (connect()/icmp/FIN)"),
		arg_interface	 = arg_str0("i", "interface", "<INTERFACE>",
									"Network interface to scan from, e.g lo"),
		arg_ports_string = arg_str0(
			"p", "port", "<PORTS>",
			"Define what port(s) to scan, e.g -p 22; -p 1-100; -p 22,53,23"),
		arg_target = arg_str1("t", "target", "<IPv4/6,Domain>",
							  "Target IPv4/IPv6/domain (Specify :4 or :6 after "
							  "domain to fetch specific adress type)"),
		arg_timeout =
			arg_int0("w", "timeout", "<TIME>",
					 "The max time the program give to a specific port"),
		arg_threads =
			arg_int0("n", "threads", "<THREADS", "Amount of threading workers"),
		end = arg_end(20),
	};

	nerrors = arg_parse(argc, argv, argtable);

	/*
		===========================================================
		Special Arguments
		===========================================================
	*/

	/* special case: '--help' takes precedence over error reporting */
	if (arg_help->count > 0) {
		PRINT("Usage: %s", progname);
		arg_print_syntax(stdout, argtable, "\n");
		PRINT("Demonstrate command-line parsing in argtable3.\n\n");
		arg_print_glossary(stdout, argtable, "  %-25s %s\n");
		exit(1);
	}

	if (arg_version->count == true) {
		PRINT("Version -> %s\n", VERSION);
		exit(2);
	}

	/*
		===========================================================
		Argument error handling
		===========================================================
	*/

	/* If the parser returned any errors then display them and exit */
	if (nerrors > 0) {
		/* Display the error details contained in the arg_end struct.*/
		arg_print_errors(stdout, end, progname);
		PRINT("Try '%s --help' for more information.\n", progname);
		exit(3);
	}

	/*
		===========================================================
		VARIBLES INITIATION & HANDLE PARAMETERS
		===========================================================
	*/
	g_verbose_enabled = arg_verbose->count > 0 ? true : false;

	bool parse_services = arg_parse_services > 0 ? true : DEFAULT_FORCE_PARSE;

	unsigned short closed_ports		  = 0;
	unsigned short error_ports		  = 0;
	double		   total_time_scanned = 0;

	unsigned short *ports	 = NULL;
	unsigned int	port_len = 0;

	scan_arg_t scan_arg;
	memset(&scan_arg, 0, sizeof(scan_arg));

	if (str_to_ipv4(&scan_arg.addr, (char *)*arg_target->sval) == 0) {
		VERBOSE_MESSAGE("Target (IPv4): %s \n", *arg_target->sval);

	} else if (str_to_ipv6(&scan_arg.addr, (char *)*arg_target->sval) == 0) {
		VERBOSE_MESSAGE("Target (IPv6): %s \n", *arg_target->sval);

	} else if (fetch_domain((char *)*arg_target->sval, &scan_arg.addr) == 0) {

		if (scan_arg.addr.ss_family == AF_INET) {
			VERBOSE_MESSAGE("Target (IPv4): %s \n", *arg_target->sval);

		} else if (scan_arg.addr.ss_family == AF_INET6) {
			VERBOSE_MESSAGE("Target (IPv6): %s \n", *arg_target->sval);
		}
	} else {
		ERR_PRINT("Could not determine what type of address, %s\n",
				  *arg_target->sval);
		exit(4);
	}

	unsigned short threads =
		arg_threads->count > 0 ? *arg_threads->ival : DEFAULT_THREAD_WORKERS;

	bool no_ping = arg_no_ping->count > 0 ? true : DEFAULT_NO_PING;

	scan_arg.timeout =
		arg_timeout->count > 0 ? *arg_timeout->ival : DEFAULT_TIMEOUT;

	scan_func_t scan_info;

	if (arg_method->count > 0) {
		if (strcmp(*arg_method->sval, "conn") == 0) {
			scan_info.scan_func			= connect_scan;
			scan_info.transfer_protocol = TCP_t;
			scan_info.needs_root		= false;
			scan_info.name				= "Three way handshake/connect()";

		} else if (strcmp(*arg_method->sval, "icmp") == 0) {
			scan_info.scan_func			= icmp_responce_scan;
			scan_info.transfer_protocol = UDP_t;
			scan_info.needs_root		= true;
			scan_info.name				= "ICMP echo reply";

		} else if (strcmp(*arg_method->sval, "syn") == 0) {
			scan_info.scan_func			= SYN_scan;
			scan_info.transfer_protocol = TCP_t;
			scan_info.needs_root		= true;
			scan_info.name				= "SYN scan";

		} else { // connect to use as backup
			ERR_PRINT("Invalid scanning method\n");
			exit(5);
		}
	}

	/*
		===========================================================

		===========================================================
	*/

	if (setup_services(parse_services) == 1) {
		VERBOSE_MESSAGE("Services already parsed\n");
	}

	scan_arg.interface.s_addr.ss_family = scan_arg.addr.ss_family;

	/*
		===========================================================
		VERBOSE ARBUMENTS OUTPUT
		===========================================================
	*/

	VERBOSE_MESSAGE("Scanning method: %s \n", scan_info.name);

	VERBOSE_MESSAGE("Pinging before scanning: %s \n",
					(!no_ping && is_root()) ? "Yes" : "No");
	VERBOSE_MESSAGE("Thread Workers: %d \n", threads);
	VERBOSE_MESSAGE("Timeout (s): %d \n", scan_arg.timeout);

	// Ugly ass spagetti code!!!!
	if (arg_interface->count <= 0) {
		VERBOSE_MESSAGE("No network interface set \n");

		// If root and want to ping
		if (!no_ping && is_root()) {
			VERBOSE_MESSAGE(" -> Guessing interface by pinging target! \n");

			if (guess_interface(scan_arg.addr, &scan_arg.interface) < 0) {
				ERR_PRINT("Failed to ping target, using the --no-ping flag "
						  "will skip this step!\n");
			} else {
				goto interface_end;
			}
		}
		VERBOSE_MESSAGE(" -> Using first interface, please specify interface "
						"if its wrong \n");

		get_first_network_interface(&scan_arg.interface);

	} else { // If the user passes a interface
		strcpy(scan_arg.interface.name, *arg_interface->sval);
		if (verify_interface(&scan_arg.interface) < 1) {
			ERR_PRINT("The given network interface does not exist: %s\n",
					  *arg_interface->sval);
			exit(6);
		}
	}

interface_end:
	VERBOSE_MESSAGE("Network interface: %s\n", scan_arg.interface.name);

	/*
		===========================================================
	*/

	port_len = parse_ports(arg_ports_string->count > 0 ? *arg_ports_string->sval
													   : DEFAULT_PORT_INTERVAL,
						   &ports);

	if (port_len == 0) {
		ERR_PRINT("Failed to parse ports\n");
		exit(7);
	}

	PRINT("Scanning on ports: %s (%d)\n",
		  arg_ports_string->count > 0 ? *arg_ports_string->sval
									  : DEFAULT_PORT_INTERVAL,
		  port_len);

	scan_result_t scan_result[port_len];

	memset(scan_result, -1, sizeof(scan_result));

	// This will be where the scanning starts
	ui_line("Starting Scan", '=', TERMINAL_WIDTH);

	multithread_scanning(&scan_info, &scan_arg, scan_result, ports, port_len,
						 threads);

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

		find_port(scan_info.transfer_protocol, ports[i], &service_buff);
		sprintf(port_buff, "%u/%s", ports[i],
				get_transfer_layer_string(scan_info.transfer_protocol));
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

arg_exit:
	if (ports) {
		free(ports);
	}
	arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
	return exitcode;
}
