#include "../../include/capture/next_best_packet.h"
#include <pthread.h>

typedef struct{
    pcap_t *package_handle;
    unsigned int timeout_s;
}exit_pcap_loop_arg;

void * exit_pcap_loop(void * arg) {
    exit_pcap_loop_arg * in_arg = (exit_pcap_loop_arg *)arg;
    sleep(in_arg->timeout_s);

    if (in_arg->package_handle != NULL) {
        pcap_breakloop(in_arg->package_handle);
    }
    return NULL;
}

void loop_back(net_packet * arg ,const struct pcap_pkthdr *packet_header, const unsigned char *payload) {
    // We need to copy the value, not the pointer
    arg->packet_header = calloc(1, sizeof(struct pcap_pkthdr));
    if (arg->packet_header == NULL) {
        ERR_PRINT("%s\n", "Failed to allocate memory in thread for next_best_packet");
    }
    arg->packet_payload = calloc(packet_header->len, sizeof(char));
    if (arg->packet_payload == NULL) {
        ERR_PRINT("%s\n", "Failed to allocate memory in thread for next_best_packet");
    }
    memcpy(arg->packet_payload, payload, packet_header->len);
    memcpy(arg->packet_header, packet_header, sizeof(struct pcap_pkthdr));
}

// ONLY FOR ONE PACKET
net_packet * next_best_packet(const char * network_interface,char * filter, int timeout) {
    char errbuff[PCAP_ERRBUF_SIZE]; // Error Buffer
    pcap_t *package_handle = NULL;
    int snap_len = MAX_PACKET_SIZE;
    int promisc = 1;
    const u_char *packet;
    struct pcap_pkthdr *header;

    net_packet * return_arg = calloc(1, sizeof(net_packet));
    if (return_arg == NULL) {
        ERR_PRINT("%s\n", "Failed to allocate memory for return_arg");
        return NULL;
    }

    struct bpf_program pcap_filter;

    package_handle = pcap_open_live(network_interface, snap_len, promisc, 500, errbuff);


    if (filter != NULL) {
        if (pcap_compile(package_handle, &pcap_filter, filter, 0, PCAP_NETMASK_UNKNOWN) < 0)  {
            ERR_PRINT("%s %s\n", "Bad filter -", pcap_geterr(package_handle));
            pcap_close(package_handle);

            free(return_arg);
            return NULL;
        }
        if (pcap_setfilter(package_handle, &pcap_filter) < 0) {
            ERR_PRINT("%s %s\n", "Error setting filter -", pcap_geterr(package_handle));
            pcap_freecode(&pcap_filter);
            pcap_close(package_handle);

            free(return_arg);
            return NULL;
        }

    }

    pthread_t thread_id;

    exit_pcap_loop_arg timeout_args;
    timeout_args.timeout_s = timeout;
    timeout_args.package_handle = package_handle;
    // Timeout counter if packages takes longer that expected
    if (pthread_create(&thread_id, NULL, exit_pcap_loop, &timeout_args) != 0) {
        pcap_close(package_handle);
        ERR_PRINT("%s\n", "Failed to create thread", NULL);
        return NULL;
    }

    // Start scanning for matching packages
    pcap_dispatch(package_handle, 1, (pcap_handler) loop_back, (unsigned char *) return_arg);
    pcap_close(package_handle);

    pthread_cancel(thread_id);
    // This it to make sure the "sleep" thread exists as expected
    // pthread_join(thread_id, NULL);



    pcap_freecode(&pcap_filter);
    return return_arg;
}
