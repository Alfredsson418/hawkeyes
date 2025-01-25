#include "../../../include/other/headers/tcp.h"

int tcp_hdr_setup(struct tcphdr *tcp_hdr, unsigned short src_port,
                  unsigned short dst_port, unsigned int seq_id,
                  struct sockaddr_storage *src_addr,
                  struct sockaddr_storage *dst_addr) {
    // Used for calculating checksum

    tcp_hdr->source  = htons(src_port);
    tcp_hdr->dest    = htons(dst_port);
    tcp_hdr->seq     = htonl(seq_id);
    tcp_hdr->ack_seq = 0;
    tcp_hdr->res1    = 0;
    tcp_hdr->res2    = 0;
    tcp_hdr->doff    = sizeof(struct tcphdr) / 4;
    tcp_hdr->window  = htons(65495);

    tcp_hdr->fin   = 0;
    tcp_hdr->syn   = 0;
    tcp_hdr->rst   = 0;
    tcp_hdr->psh   = 0;
    tcp_hdr->ack   = 0;
    tcp_hdr->urg   = 0;
    tcp_hdr->check = 0;

    if (src_addr->ss_family == AF_INET) {
        struct pseudo_header pseudo_hdr;

        pseudo_hdr.source_address =
            ((struct sockaddr_in *)src_addr)->sin_addr.s_addr;
        pseudo_hdr.dest_address =
            ((struct sockaddr_in *)dst_addr)->sin_addr.s_addr;
        pseudo_hdr.placeholder = 0;
        pseudo_hdr.protocol    = IPPROTO_TCP;
        pseudo_hdr.tcp_length  = htons(sizeof(struct tcphdr));

        unsigned int pseudo_size =
            sizeof(struct pseudo_header) + sizeof(struct tcphdr);
        char pseudogram[pseudo_size];

        memcpy(pseudogram, &pseudo_hdr, sizeof(struct pseudo_header));
        memcpy(pseudogram + sizeof(struct pseudo_header), tcp_hdr,
               sizeof(struct tcphdr));

        tcp_hdr->check = checksum(pseudogram, pseudo_size);
    } else if (src_addr->ss_family == AF_INET6) {
        struct ip6_pseudo_header psudo_hdr;

    } else {
        return 0;
    }

    return 0;
}
