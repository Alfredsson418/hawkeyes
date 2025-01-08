#include "../../include/other/terminal_args.h"

// Program documentation.
const char terminal_docs[] = "Scan for open ports on target device";

// This should be formated, but the {0} at the failes this,
const struct argp_option terminal_options[] = {
    {"verbose", 'v', 0, 0, "Verbose output"},
    {"quiet", 501, 0, 0, "No terminal output"},
    {"method", 'm', "METHOD(connect()/icmp/FIN)", 0, ""},
    {"interface", 'i', "INTERFACE", 0,
     "Network interface to scan from, e.g lo"},
    {"port", 'p', "PORT(S)", 0,
     "Define what port(s) to scan, e.g -p 22; -p 1-100; -p 22,53,23"},
    {"target", 't', "IPv4", 0, "Target IPv4"},
    {"timeout", 'w', "TIME(sec)", 0, "Timeout"},
    {"no-ping", 503, 0, 0, "Do not ping the target"},
    {"threading-workers", 'n', "WORKERS", 0, "Amount of threading workers"},
    {0}
};

struct argp terminal_argp = {terminal_options, terminal_parse_opt, 0,
                             terminal_docs};

error_t terminal_parse_opt(int key, char *arg, struct argp_state *state) {
    /*
        Get the input argument from capture_argp_parse, which we
        know is a pointer to our arguments structure.
    */
    struct terminal_args *arguments = state->input;

    switch (key) {
    case 'v':
        g_verbose_enabled = 1;
        break;
    case 501:
        g_no_terminal_output = 1;
        break;
    case 'i':
        if (!arg) {
            break;
        }
        if (strlen(arg) > INTERFACE_LEN) {
            ERR_PRINT("Interface name to long\n");
            break;
        }
        strncpy(arguments->interface, arg, strlen(arg));
        break;
    case 'p':
        if (!arg) {
            break;
        }
        if (strlen(arg) > PORTS_FORMAT_LEN) {
            ERR_PRINT("Port input to long\n");
            break;
        }
        strncpy(arguments->ports_format, arg, strlen(arg));
        break;
    case 't':
        memset(&arguments->address, 0, sizeof(struct sockaddr_storage));

        if (str_to_ipv4(&arguments->address, arg) != -1) {
            break;
        } else if (str_to_ipv6(&arguments->address, arg) != -1) {
            break;
        } else {
            str_to_ipv4(&arguments->address, "255.255.255.255");
        }
        break;
    case 'w':
        arguments->timeout = atoi(arg);
        break;
    case 'm':
        if (strcmp(arg, "conn") == 0) {
            arguments->scan_info.scan_func         = connect_scan;
            arguments->scan_info.transfer_protocol = TCP_t;
            arguments->scan_info.needs_root        = false;
            arguments->scan_info.name = "Three way handshake/connect()";
        } else if (strcmp(arg, "icmp") == 0) {
            arguments->scan_info.scan_func         = icmp_responce_scan;
            arguments->scan_info.transfer_protocol = UDP_t;
            arguments->scan_info.needs_root        = true;
            arguments->scan_info.name              = "ICMP echo reply";

            // } else if (strcmp(arg, "half-sync") == 0) {
            // arguments->scan_method = SCAN_HALF_SYNC_t;

        } else { // connect to use as backup
            ERR_PRINT("Invalid scanning method\n");
        }
        break;
    case 503:
        arguments->no_ping = true;
        break;
    case 'n':
        arguments->thread_workers = atoi(arg);
        break;
    default:
        return ARGP_ERR_UNKNOWN;
    }
    return 0;
}
