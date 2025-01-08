        (icmp[30:2] == %#06x): This condition checks if the src port of the ICMP
       packet SENT matches the port variable. The %#06x format specifier ensures
       that the port value is formatted as a zero-padded, 6-character wide
        hexadecimal number with a 0x prefix.

        (icmp[0] == 3): This condition checks if the first byte of the ICMP
        packet is equal to 3, which typically indicates a "Destination
       Unreachable" message in ICMP.

        (icmp[1] == 3): This condition checks if the second byte of the
        ICMP packet is equal to 3, which usually specifies the "Port
       Unreachable" code within the "Destination Unreachable" message.
