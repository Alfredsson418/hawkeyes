#include "../../include/capture/packet_info.h"

// Callback function for pcap_loop
void packet_handler(struct capture_arguments * arguments ,const struct pcap_pkthdr *packet_header, const unsigned char *packet) {

    /*
        sniff_ethernet 	X
        sniff_ip 	X + SIZE_ETHERNET
        sniff_tcp 	X + SIZE_ETHERNET + {IP header length}
        payload 	X + SIZE_ETHERNET + {IP header length} + {TCP header length}
    
    */
    
    // String does not change
    char * string = calloc(strlen(arguments->format) + 1, sizeof(char));
    memset(string, '\0', sizeof(string));
    strcpy(string, arguments->format);
    if (strstr(string, "\\n")) {
        replace_substring(string, "\\n", "\n");
    }
    if (strstr(string, "\\t")) {
        replace_substring(string, "\\t", "\t");
    }
    if (strstr(string, "\\v")) {
        replace_substring(string, "\\v", "\v");
    }

    packet_header_info(packet_header, &string);

    /*
        The size of the Ethernet header is typically 14 bytes (sizeof(struct ether_header)),
        and the size of the IP header can be calculated from the ihl field of the iphdr struct,
        which represents the header length in 32-bit words. To convert this to bytes, you multiply by 4.
    */
    l2_packet_info(packet, &string);
    l3_packet_info(packet, &string);
    PRINT("%s\n", string);
    
    if (arguments->hexdump) {
        // unsigned char *payload = (unsigned char *)(packet + sizeof(struct ether_header) + sizeof(struct iphdr));
        // int payload_length = packet_header->len - sizeof(struct ether_header) + sizeof(struct iphdr);
        PRINT("%s\n", "HEXDUMP:");
        hexdump((unsigned *) packet, packet_header->len, 16);
    }
    
    free(string);
}   

void packet_header_info(const struct pcap_pkthdr *packet_header, char **format) {
    
    char time_sec[20]; // Enough for a timestamp
    char origin_len[20]; // Enough for an int
    char capture_len[20]; // Enough for an int

    sprintf(time_sec, "%ld.%02ld", packet_header->ts.tv_sec, packet_header->ts.tv_usec);
    sprintf(origin_len, "%d", packet_header->len);
    sprintf(capture_len, "%d", packet_header->caplen);


    replace_format(format, "{head-capture-len}", capture_len);
    replace_format(format, "{head-origin-len}", origin_len);
    replace_format(format, "{head-time}", time_sec);
}

void l2_packet_info(const unsigned char *packet, char **format) {
    char mac_src[18] = {0};
    char mac_dst[18] = {0};
    
    struct ether_header *eth = (struct ether_header *)packet;
    

    if (eth != NULL) {
        bin_to_mac(eth->ether_dhost, mac_dst);
        bin_to_mac(eth->ether_shost, mac_src);
    }

    replace_format(format, "{mac-src}", mac_src);
    replace_format(format, "{mac-dst}", mac_dst);
    replace_format(format, "{mac-prot}", determine_packet_protocol(eth->ether_type, 2));
}

void l3_packet_info(const unsigned char *packet, char **format) {
    /*
        unsigned char ihl: The Internet Header Length (IHL) field. This represents the length of the IP header in 32-bit words. The minimum value is 5, which corresponds to a header length of 20 bytes.
        unsigned char version: The version of the IP protocol. For IPv4, this is always 4.
        unsigned char tos: The Type of Service (ToS) field. This field is used to specify the quality of service desired for the packet.
        unsigned short tot_len: The total length of the IP packet, including the header and the data, in bytes.
        unsigned short id: An identifier to aid in assembling the fragments of a datagram.
        unsigned short frag_off: The fragment offset field, measured in units of 8-byte blocks.
        unsigned char ttl: The Time To Live (TTL) field. This field is used to limit the lifespan of a packet in the network.
        unsigned char protocol: The protocol field. This field indicates the protocol used in the payload of the IP packet.
        unsigned short check: The checksum field. This field is used for error-checking of the header.
        unsigned int saddr: The source IP address.
        unsigned int daddr: The destination IP address.
    */
    struct ether_header *eth = (struct ether_header *)packet;
    struct iphdr * ip = (struct iphdr *)(packet + sizeof(struct ether_header)); // Skip the Ethernet header;

    char src_ip[INET_ADDRSTRLEN];
    char dst_ip[INET_ADDRSTRLEN];
    char str[4] = {0};
    char *ip_protocol = NULL;
    struct in_addr src_addr, dst_addr;
    char ttl [4]= {0};
    char version[4] = {0};

    if (ip != NULL && ntohs(eth->ether_type) == 0x0800) {
        dst_addr.s_addr = ip->daddr;
        strncpy(dst_ip, inet_ntoa(dst_addr), INET_ADDRSTRLEN);

        // inet_ntoa will overwrite if not copied to other value
        src_addr.s_addr = ip->saddr;
        strncpy(src_ip, inet_ntoa(src_addr), INET_ADDRSTRLEN);
        
        ip_protocol = determine_packet_protocol(ip->protocol, 3);
        sprintf(str, "%u", ip->protocol);
        sprintf(ttl, "%u", ip->ttl);
        sprintf(version, "%u", ip->version);
    } 
    
    replace_format(format, "{ipv4-src}", src_ip);
    replace_format(format, "{ipv4-dst}", dst_ip);
    replace_format(format, "{l3-prot}", ip_protocol);
    replace_format(format, "{l3-prot-vers}", version);
    replace_format(format, "{l3-prot-num}", str);
    replace_format(format, "{ttl}", ttl);
}

