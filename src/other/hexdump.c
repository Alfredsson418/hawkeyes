#include "../../include/other/hexdump.h"

void hexdump (const unsigned char * payload, const int len, int perLine) {
    // Silently ignore silly per-line values.

    if (perLine < 4 || perLine > 64) perLine = 16;

    int i;
    unsigned char buff[perLine+1];


    // Length checks.
    if (len == 0) {
        ERR_PRINT("Hexdump: ZERO LENGTH\n");
        return;
    }
    if (len < 0) {
        ERR_PRINT("Hexdump: NEGATIVE LENGTH: %d", len);
        return;
    }

    // Process every byte in the data.
    for (i = 0; i < len; i++) {
        // Multiple of perLine means new or first line (with line offset).

        if ((i % perLine) == 0) {
            // Print buffer.
            if (i != 0) PRINT("  %s\n", buff);

            // Line number.
            PRINT("  %04x ", i);
        }

        // Hex code for the specific character.
        PRINT(" %02x", payload[i]);

        // And buffer a printable ASCII character for later.
        if ((payload[i] < 0x20) || (payload[i] > 0x7e))
            buff[i % perLine] = '.';
        else
            buff[i % perLine] = payload[i];
        buff[(i % perLine) + 1] = '\0';
    }

    // Print out empty space
    while ((i % perLine) != 0) {
        PRINT("   ");
        i++;
    }

    // Last line remaining buffer.
    PRINT("  %s\n", buff);
}
