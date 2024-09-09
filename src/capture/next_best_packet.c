#include "../../include/capture/next_best_packet.h"


typedef struct {
    int timeout;
    pcap_t * handle;
    int running;
} timeout_arg;


void * sleep_timeout(void * args) {
    timeout_arg * arg = (timeout_arg *) args;

    arg->running = 1;
    int time = 0;
    while(time < arg->timeout) {
        if (arg->running == 0) {
            break;
        }
        sleep(1);
        time++;
    }

    pcap_breakloop(arg->handle);

}



void loop_back(net_packet * arg ,const struct pcap_pkthdr *packet_header, const unsigned char *payload) {

    // We need to copy the value, not the pointer
    *(arg->packet_payload) = *payload;
    *(arg->packet_header) = *packet_header;

    PRINT("FOUND SOMETHING\n", NULL);
    

}

// ONLY FOR ONE PACKET
net_packet * next_best_packet(char * network_device ,char * filter, int timeout) {
    char errbuff[PCAP_ERRBUF_SIZE]; // Error Buffer
    pcap_t *package_handle = NULL;
    int snap_len = MAX_PACKET_SIZE;
    int promisc = 1;
    int net_dev_alloc = 0;

    net_packet * return_arg = calloc(1, sizeof(net_packet));
    if (return_arg == NULL) {
        ERR_PRINT("%s\n", "Failed to allocate memory for return_arg");
        return NULL;
    }
    return_arg->packet_payload = calloc(MAX_PACKET_SIZE + 1, sizeof(char));
    if (return_arg->packet_payload == NULL) {
        ERR_PRINT("%s\n", "Failed to allocate memory for packet_payload");
        free(return_arg);
        return NULL;
    }
    return_arg->packet_header = calloc(1, sizeof(struct pcap_pkthdr));
    if (return_arg->packet_header == NULL) {
        ERR_PRINT("%s\n", "Failed to allocate memory for packet_header");
        free(return_arg->packet_payload);
        free(return_arg);
        return NULL;
    }
    
    if (network_device == NULL) {
        network_device = get_first_network_dev();
        net_dev_alloc = 1;
    }

    struct bpf_program pcap_filter;
    
    package_handle = pcap_open_live(network_device, snap_len, promisc, 500, errbuff);

    // Compiles and setting filter
    if (package_handle == NULL) {
        ERR_PRINT("%s\n", "Error opening pcap file");
        ERR_PRINT("%s\n", errbuff);
        return NULL;
    }
    if (filter != NULL) {
        if (pcap_compile(package_handle, &pcap_filter, filter, 0, 0) == -1)  {
            ERR_PRINT("%s %s\n", "Bad filter -", pcap_geterr(package_handle));
            return NULL;
        }
        if (pcap_setfilter(package_handle, &pcap_filter) == -1) {
            ERR_PRINT("%s %s\n", "Error setting filter -", pcap_geterr(package_handle));
            return NULL;
        }
    }


    pthread_t thread_id;

    timeout_arg timeout_args;
    timeout_args.timeout = timeout;
    timeout_args.handle = package_handle;
    // Timeout counter if packages takes longer that expected
    if (pthread_create(&thread_id, NULL, sleep_timeout, &timeout_args) != 0) {
        ERR_PRINT("%s\n", "Failed to create thread", NULL);
        return NULL;
    }

    // Start scanning for matching packages
    pcap_dispatch(package_handle, 1, (pcap_handler) loop_back, (unsigned char *) return_arg);

    // This it to make sure the "sleep" thread exists as expected
    timeout_args.running = 0;
    pthread_join(thread_id, NULL);



    pcap_close(package_handle);
    pcap_freecode(&pcap_filter);
    if (net_dev_alloc == 1) {
        free_dev(network_device);
    } 
    
    return return_arg;
}