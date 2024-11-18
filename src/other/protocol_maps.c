#include "../../include/other/protocol_maps.h"

struct protocol_mapping {
    uint16_t ether_type;
    char* protocol_name;
};

const struct protocol_mapping l2_protocol_map[] = {
    {0x0800, "IPv4"}, // Internet
    {0x0806, "ARP"}, // Address Resolution Protocol
    {0x0842, "Wake-on-lan"},
    {0x2000, "Cisco Discovery Protocol"},
    {0x22EA, "Stream Reservation Protocol"},
    {0x22F0, "AVTP"}, // Audio Video Transport Protocol
    {0x22F3, "IETF TRILL Protocol"},
    {0x6002, "DEC MOP RC"},
    {0x6003, "DECnet Phase IV, DNA Routing"},
    {0x6004, "DEC LAT"},
    {0x8035, "RARP"}, // Reverse Address Resolution Protocol
    {0x8100, "IEEE 802.1Q/IEEE 802.1aq"}, // VLAN-tagged frame (IEEE 802.1Q) and Shortest Path Bridging IEEE 802.1aq with NNI compatibility
    {0x8102, "SLPP"}, // Simple Loop Prevention Protocol
    {0x8103, "VLACP"}, // Virtual Link Aggregation Control Protocol
    {0x8137, "IPX"},
    {0x8204, "QNX Qnet"},
    {0x86dd, "IPv6"},
    {0x8808, "Ethernet flow control"},
    {0x8809, "Ethernet Slow Protocols e.g. LACP"},
    {0x8819, "CobraNet"},
    {0x8847, "MPLS unicast"},
    {0x8848, "MPLS multicast"},
    {0x8863, "PPPoE Discovery Stage"},
    {0x8864, "PPPoE Session Stage"},
    {0x887B, "HomePlug 1.0 MME"},
    {0x888E, "IEEE 802.1X"},// EAP over LAN
    {0x8892, "PROFINET"},
    {0x889A, "SCSI over Ethernet"}, // HyperSCSI
    {0x88A2, "AoE"}, // ATA over Ethernet
    {0x88A4, "EtherCAT"},
    {0x88A8, "IEEE 802.1ad"}, // Service VLAN tag identifier (S-Tag) on Q-in-Q tunnel
    {0x88AB, "Ethernet Powerlink"},
    {0x88B8, "GOOSE"}, // Generic Object Oriented Substation event
    {0x88B9, "GSE"}, // Generic Substation Events
    {0x88BA, "IEC 61850"}, // SV (Sampled Value Transmission)
    {0x88BF, "MikroTik RoMON"}, // Unofficial
    {0x88CC, "LLDP"}, // Link Layer Discovery Protocol
    {0x88CD, "SERCOS III"},
    {0x88E1, "HomePlug Green PHY"},
    {0x88E3, "IEC62439-2"}, // Media Redundancy Protocol
    {0x88E5, "IEEE 802.1AE"}, // MAC security (MACsec)
    {0x88E7, "IEEE 802.1ah"}, // Provider Backbone Bridges (PBB)
    {0x88F7, "PTP"}, // Precision Time Protocol (PTP) over IEEE 802.3 Ethernet
    {0x88F8, "NC-SI"},
    {0x88FB, "PRP"}, // Parallel Redundancy Protocol
    {0x8902, "IEEE 802.1ag CFM / ITU-T Y.1731"}, // IEEE 802.1ag Connectivity Fault Management (CFM) Protocol / ITU-T Recommendation Y.1731 (OAM)
    {0x8906, "FCoE"}, // Fibre Channel over Ethernet
    {0x8914, "FCoE Initialization Protocol"},
    {0x8915, "RoCE"}, //RDMA over Converged Ethernet
    {0x891D, "TTE"}, // TTEthernet Protocol Control Frame
    {0x893a, "IEEE 1905.1"},
    {0x892F, "HSR"}, // High-availability Seamless Redundancy
    {0x9000, "Ethernet Configuration Testing Protocol"},
    {0xF1C1, "Redundancy Tag"} // Redundancy Tag (IEEE 802.1CB Frame Replication and Elimination for Reliability)
};


// https://www.iana.org/assignments/protocol-numbers/protocol-numbers.xhtml
const struct protocol_mapping l3_protocol_map[] = {
    {0x00, "HOPOPT"}, // IPv6 Hop-by-Hop Option
    {0x01, "ICMP"}, // Internet Control Message
    {0x02, "IGMP"}, // Internet Group Management
    {0x03, "GGP"}, // Gateway-to-Gateway
    {0x04, "IPv4"}, // IPv4 encapsulation
    {0x05, "ST"}, // Stream
    {0x06, "TCP"}, // Transmission Control
    {0x07, "CBT"}, // CBT
    {0x08, "EGP"}, // Exterior Gateway Protocol
    {0x09, "IGP"}, // any private interior gateway (used by Cisco for their IGRP)
    {0x0A, "BBN-RCC-MON"}, // BBN RCC Monitoring
    {0x0B, "NVP-II"}, // Network Voice Protocol
    {0x0C, "PUP"}, // PUP
    {0x0D, "ARGUS"}, // ARGUS (deprecated)
    {0x0E, "EMCON"}, // EMCON
    {0x0F, "XNET"}, // Cross Net Debugger
    {0x10, "CHAOS"}, // Chaos
    {0x11, "UDP"}, // User Datagram
    {0x12, "MUX"}, // Multiplexing
    {0x13, "DCN-MEAS"}, // DCN Measurement Subsystems
    {0x14, "HMP"}, // Host Monitoring
    {0x15, "PRM"}, // Packet Radio Measurement
    {0x16, "XNS-IDP"}, // XEROX NS IDP
    {0x17, "TRUNK-1"},
    {0x18, "TRUNK-2"},
    {0x19, "LEAF-1"},
    {0x1A, "LEAF-2"},
    {0x1B, "RDP"}, // Reliable Data Protocol
    {0x1C, "IRTP"}, // Internet Reliable Transaction
    {0x1D, "ISO-TP4"}, // ISO Transport Protocol Class 4
    {0x1E, "NETBLT"}, // Bulk Data Transfer Protocol
    {0x1F, "MFE-NSP"}, // MFE Network Services Protocol
    {0x20, "MERIT-INP"}, // MERIT Internodal Protocol
    {0x21, "DCCP"}, // Datagram Congestion Control Protocol
    {0x22, "3PC"}, // Third Party Connect Protocol
    {0x23, "IDPR"}, // Inter-Domain Policy Routing Protocol
    {0x24, "XTP"},
    {0x25, "DDP"}, // Datagram Delivery Protocol
    {0x26, "IDPR-CMTP"}, // IDPR Control Message Transport Proto
    {0x27, "TP++"}, // TP++ Transport Protocol
    {0x28, "IL"}, // IL Transport Protocol
    {0x29, "IPv6"}, // IPv6 encapsulation
    {0x2A, "SDRP"}, // Source Demand Routing Protocol
    {0x2B, "IPv6-Route"}, // Routing Header for IPv6
    {0x2C, "IPv6-Frag"}, // Fragment Header for IPv6
    {0x2D, "IDRP"}, // Inter-Domain Routing Protocol
    {0x2E, "RSVP"}, // Reservation Protocol
    {0x2F, "GRE"}, // Generic Routing Encapsulation
    {0x30, "DSR"}, // Dynamic Source Routing Protocol
    {0x31, "BNA"},
    {0x32, "ESP"}, // Encap Security Payload
    {0x33, "AH"}, // Authentication Header
    {0x34, "I-NLSP"}, // Integrated Net Layer Security TUBA
    {0x35, "SWIPE"}, // IP with Encryption (deprecated)
    {0x36, "NARP"}, // NBMA Address Resolution Protocol
    {0x37, "Min-IPv4"}, // Minimal IPv4 Encapsulation
    {0x38, "TLSP"}, // Transport Layer Security Protocol using Kryptonet key management
    {0x39, "SKIP"},
    {0x3A, "IPv6-ICMP"},
    {0x3B, "IPv6-NoNxt"}, // No Next Header for IPv6
    {0x3C, "IPv6-Opts"}, // Destination Options for IPv6
    // {0x3D, ""}, // any host internal protocol
    {0x3E, "CFTP"},
    // {0x3F, ""}, // any local network
    {0x40, "SAT-EXPAK"}, // SATNET and Backroom EXPAK
    {0x41, "KRYPTOLAN"},
    {0x42, "RVD"}, // MIT Remote Virtual Disk Protocol
    {0x43, "IPPC"}, // Internet Pluribus Packet Core
    // {0x44, ""}, //any distributed file system
    {0x45, "SAT-MON"}, // SATNET Monitoring
    {0x46, "VISA"},
    {0x47, "IPCV"}, // Internet Packet Core Utility
    {0x48, "CPNX"}, // Computer Protocol Network Executive
    {0x49, "CPHB"}, // Computer Protocol Heart Beat
    {0x4A, "WSN"}, // Wang Span Network
    {0x4B, "PVP"}, // Packet Video Protocol
    {0x4C, "BR-SAT-MON"}, // Backroom SATNET Monitoring
    {0x4D, "SUN-ND"}, // SUN ND PROTOCOL-Temporary
    {0x4E, "WB-MON"}, // WIDEBAND Monitoring
    {0x4F, "WB-EXPAK"}, // WIDEBAND EXPAK
    {0x50, "ISO-IP"}, // ISO Internet Protocol
    {0x51, "VMTP"},
    {0x52, "SECURE-VMTP"},
    {0x53, "VINES"},
    {0x54, "IPTM"}, // Internet Protocol Traffic Manager
    {0x55, "NSFNET-IGP"},
    {0x56, "DGP"}, // Dissimilar Gateway Protocol
    {0x57, "TCF"},
    {0x58, "EIGRP"},
    {0x59, "OSPFIGP"},
    {0x5A, "Sprite-RPC"},
    {0x5B, "LARP"}, // Locus Address Resolution Protocol
    {0x5C, "MTP"}, // Multicast Transport Protocol
    {0x5D, "AX.25"}, // AX.25 Frames
    {0x5E, "IPIP"}, // IP-within-IP Encapsulation Protocol
    {0x5F, "MICP"}, // Mobile Internetworking Control Pro. (deprecated)
    {0x60, "SCC-SP"}, // Semaphore Communications Sec. Pro.
    {0x61, "ETHERIP"}, // Ethernet-within-IP Encapsulation
    {0x62, "ENCAP"}, // Encapsulation Header
    // {0x63, ""}, // any private encryption scheme
    {0x64, "GMTP"},
    {0x65, "IFMP"}, // Ipsilon Flow Management Protocol
    {0x66, "PNNI"},
    {0x67, "PIM"}, // Protocol Independent Multicast
    {0x68, "ARIS"},
    {0x69, "SCPS"},
    {0x6A, "QNX"},
    {0x6B, "A/N"}, // Active Networks
    {0x6C, "IPComp"}, // IP Payload Compression Protocol
    {0x6D, "SNP"}, // Sitara Networks Protocol
    {0x6E, "Compaq-Peer"}, // Compaq Peer Protocol
    {0x6F, "IPX-in-IP"}, // IPX in IP
    {0x70, "VRRP"}, // Virtual Router Redundancy Protocol
    {0x71, "PGM"}, // PGM Reliable Transport Protocol
    // {0x72, ""}, // any 0-hop protocol
    {0x73, "L2TP"}, // Layer Two Tunneling Protocol
    {0x74, "DDX"}, // D-II Data Exchange (DDX)
    {0x75, "IATP"}, // Interactive Agent Transfer Protocol
    {0x76, "STP"}, // Schedule Transfer Protocol
    {0x77, "SRP"}, // SpectraLink Radio Protocol
    {0x78, "UTI"},
    {0x79, "SMP"}, // Simple Message Protocol
    {0x7A, "SM"}, // Simple Multicast Protocol (deprecated)
    {0x7B, "PTP"}, // Performance Transparency Protocol
    {0x7C, "ISIS over IPv4"},
    {0x7D, "FIRE"},
    {0x7E, "CRTP"}, // Combat Radio Transport Protocol
    {0x7F, "CRUDP"}, // Combat Radio User Datagram
    {0x80, "SSCOPMCE"},
    {0x81, "IPLT"},
    {0x82, "SPS"}, // Secure Packet Shield
    {0x83, "PIPE"}, // Private IP Encapsulation within IP
    {0x84, "SCTP"}, // Stream Control Transmission Protocol
    {0x85, "FC"}, // Fibre Channel
    {0x86, "RSVP-E2E-IGNORE"},
    {0x87, "Mobility Header"},
    {0x88, "UDPLite"},
    {0x89, "MPLS-in-IP"},
    {0x8A, "manet"}, // MANET Protocols
    {0x8B, "HIP"}, // Host Identity Protocol
    {0x8C, "Shim6"},
    {0x8D, "WESP"}, // Wrapped Encapsulating Security Payload
    {0x8E, "ROHC"}, // Robust Header Compression
    {0x8F, "Ethernet"},
    {0x90, "AGGFRAG"}, // AGGFRAG encapsulation payload for ESP
    {0x91, "NSH"}, // Network Service Header
    // This goes up to 255 but the rest are unassigned
};

// Determine
char * determine_packet_protocol(uint16_t packet_protocol, int layer) {
    // https://en.wikipedia.org/wiki/EtherType
    /*
        EtherType is a mechanism used at Layer 2 to facilitate the
        correct interpretation and processing of data at Layer 3.
        The "map" can be found in the header file
    */
    switch (layer)
    {
    case 2: // Layer 2 (Ethertype)
        for (int i = 0; i < sizeof(l2_protocol_map) / sizeof(l2_protocol_map[0]); i++) {
            // ntohs is used to get a decimal
            if (ntohs(packet_protocol) == l2_protocol_map[i].ether_type) {
                return l2_protocol_map[i].protocol_name;
            }
        }
        return "Unknown";

    case 3: // Layer 3 (IP)
        for (int i = 0; i < sizeof(l3_protocol_map) / sizeof(l3_protocol_map[0]); i++) {
            if (packet_protocol == l3_protocol_map[i].ether_type) {
                return l3_protocol_map[i].protocol_name;
            }
        }
        return "Unknown";

    default:
        return "Unknown";
    }
}
