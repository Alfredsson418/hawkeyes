#include "../../include/other/terminal_args.h"

// Program documentation.
const char terminal_docs[] = "Scan for open ports on target device";


const struct argp_option terminal_options[] = {
    {"verbose", 'v', 0, 0, "Verbose output"},
    {"quiet", 501, 0, 0, "No terminal output"},
    {"method", 'm', "METHOD(tcp/udp/half-sync)", 0, ""},
    {"interface", 'i', "INTERFACE", 0, "Network interface to scan from, e.g lo"},
    {"port", 'p', "PORT(S)", 0, "Define what port(s) to scan, e.g -p 22; -p 1-100; -p 22,53,23"},
    {"target", 't', "IPv4", 0, "Target IPv4"},
    {"timeout", 'w', "TIME(sec)", 0, "Timeout"},
    {"no-ping", 503, 0, 0, "Do not ping the target"},
    {"threading-workers", 'n', "WORKERS", 0, "Amount of threading workers"},
    { 0 }
};

struct argp terminal_argp = { terminal_options, terminal_parse_opt, 0, terminal_docs };


error_t terminal_parse_opt(int key, char *arg, struct argp_state *state){
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
        if (!arg) {break;}
        if (strlen(arg) > INTERFACE_LEN) {
            ERR_PRINT("Interface name to long\n");
            break;
        }
        strncpy(arguments->interface, arg, strlen(arg));
        break;
    case 'p':
        if (!arg) {break;}
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
        if (strcmp(arg, "tcp") == 0) {
            arguments->scan_method = SCAN_TCP_t;
            break;
        } else if (strcmp(arg, "udp") == 0) {
            arguments->scan_method = SCAN_UDP_t;
            break;
        } else if (strcmp(arg, "half-sync") == 0) {
            arguments->scan_method = SCAN_HALF_SYNC_t;
            break;
        }
        arguments->scan_method = 0;
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
