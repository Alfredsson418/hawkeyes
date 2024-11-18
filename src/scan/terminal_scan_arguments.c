#include "../../include/scan/terminal_scan_arguments.h"

// Program documentation.
const char terminal_scan_docs[] = "Scan for open ports on target device";


const struct argp_option terminal_scan_options[] = {
    {"verbose", 'v', 0, 0, "Verbose output"},
    {"quiet", 501, 0, 0, "No terminal output"},
    {"protocol", 'm', "METHOD(tcp/udp/half-sync)", 0, ""},
    {"interface", 'i', "INTERFACE", 0, "Network interface to scan from, e.g lo"},
    {"port", 'p', "PORT(S)", 0, "Define what port(s) to scan, e.g -p 22; -p 1-100; -p 22,53,23"},
    {"target", 't', "IP", 0, "Target device, needs to correlate with 'device', e.g 127.0.0.1 works only for lo"},
    {"timeout", 'w', "TIME(sec)", 0, "Timeout"},
    {"no-ping", 503, 0, 0, "Do not ping the target"},
    {"threading-workers", 'n', "WORKERS", 0, "Amount of threading workers"},
    { 0 }
};

struct argp terminal_scan_argp = { terminal_scan_options, terminal_scan_parse_opt, 0, terminal_scan_docs };


error_t terminal_scan_parse_opt(int key, char *arg, struct argp_state *state){
    /*
        Get the input argument from capture_argp_parse, which we
        know is a pointer to our arguments structure.
    */
    struct terminal_scan_arguments *arguments = state->input;

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
        inet_pton(AF_INET, arg, &(arguments->target)); // Add error handling
        break;
    case 'w':
        arguments->timeout = atoi(arg);
        break;
    case 'm':
        if (strcmp(arg, "tcp") == 0) {
            arguments->scan_protocol = TCP_NUM;
            break;
        } else if (strcmp(arg, "udp") == 0) {
            arguments->scan_protocol = UDP_NUM;
            break;
        } else if (strcmp(arg, "half-sync") == 0) {
            arguments->scan_protocol = HALF_SYNC_NUM;
            break;
        }
        arguments->scan_protocol = 0;
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