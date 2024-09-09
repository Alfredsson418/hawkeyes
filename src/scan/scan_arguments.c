#include "../../include/scan/scan_arguments.h"

// Program documentation.
const char scan_docs[] = "Scan for open ports on target device";


const struct argp_option scan_options[] = {
    {"verbose", 'v', 0, 0, "Verbose output"},
    {"quiet", 501, 0, 0, "No terminal output"},
    {"device", 502, "DEVICE", 0, "Source network device to scan from, e.g lo"},
    {"port", 'p', "PORT(S)", 0, "Define what port(s) to scan, e.g -p 22; -p 1-100; -p 22,53,23"},
    {"target", 't', "IP", 0, "Target device, needs to correlate with 'device', e.g 127.0.0.1 works only for lo"},
    {"timeout", 'w', "TIME(sec)", 0, "Timeout"},
    { 0 }
};

struct argp scan_argp = { scan_options, scan_parse_opt, 0, scan_docs };


error_t scan_parse_opt(int key, char *arg, struct argp_state *state){
    /*  
        Get the input argument from capture_argp_parse, which we
        know is a pointer to our arguments structure.
    */
    struct scan_arguments *arguments = state->input;
    
    switch (key) {
    case 'v':
        g_verbose_enabled = 1;
        break;
    case 501:
        g_no_terminal_output = 1;
        break;
    case 502:
        arguments->device = arg;
        break;
    case 'p':
        arguments->ports_format = arg;
        break;
    case 't':
        arguments->target = arg;
        break;
    case 'w':
        arguments->timeout = atoi(arg);
        break;
    default:
        return ARGP_ERR_UNKNOWN;
    }
    return 0;
    
}