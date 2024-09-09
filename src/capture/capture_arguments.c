#include "../../include/capture/capture_arguments.h"

// Program documentation.
const char capture_docs[] = "Capture packets from and to your device.";



/*
    The OPTIONS field contains a pointer to a vector of struct argp_option’s;
    that structure has the following fields (if you assign your option
    structures using array initialization like this example, unspecified
    fields will be defaulted to 0, and need not be specified):
     NAME   – The name of this option’s long option (may be zero)
     KEY    – The KEY to pass to the PARSER function when parsing this option,
               *and* the name of this option’s short option, if it is a
               printable ascii character
     ARG    – The name of this option’s argument, if any
     FLAGS  – Flags describing this option; some of them are:
                 OPTION_ARG_OPTIONAL – The argument to this option is optional
                 OPTION_ALIAS        – This option is an alias for the
                                        previous option
                 OPTION_HIDDEN       – Don’t show this option in –help output
     DOC    – A documentation string for this option, shown in –help output
*/
/*
    ----to add----
    read timeout (in milliseconds)
    snap_len
    promisc
    pcap_file_out
*/
const struct argp_option capture_options[] = {
    {"verbose", 'v', 0, 0, "Verbose output"},
    {"quiet", 501, 0, 0, "No output to terminal (Error and Debug will still display)"},
    {"format", 500, "FORMAT", 0, "Specifies output FORMAT for packet capturing"},
    {"device", 'd', "DEVICE", 0, "Specifies the DEVICE to capture from"},
    {"hexdump", 'x', 0, 0, "Output hexdump from package"},
    {"pcap-load", 502, "FILE", 0, "Load pcap file into program"},
    {"filter", 'f', "FILTER", 0, "Filter captured traffic"},
    {"capture-amount", 504, "AMOUNT", 0, "The amount of packages you want to capture (0 too loop forever)"},
    { 0 }
};

struct argp capture_argp = { capture_options, capture_parse_opt, 0, capture_docs };


error_t capture_parse_opt(int key, char *arg, struct argp_state *state){
    /*  
        Get the input argument from capture_argp_parse, which we
        know is a pointer to our arguments structure.
    */
    struct capture_arguments *arguments = state->input;
    
    switch (key) {
    case 'v':
        g_verbose_enabled = 1;
        break;
    case 500:
        arguments->format = arg;
        break;
    case 'd':
        arguments->device = arg;
        break;
    case 'x':
        arguments->hexdump = 1;
        break;
    case 502:
        arguments->pcap_load = arg;
        break;
    case 'f':
        arguments->filter = arg;
        break;
    case 504:
        arguments->capture_amount = atoi(arg);
        break;
    case 501:
        g_no_terminal_output = 1;
        break;
    default:
        return ARGP_ERR_UNKNOWN;
    }
    return 0;
    
}