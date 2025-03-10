#include "../../include/ui/terminal_display.h"

void ui_line(char *text, char padding, int width) {
	if (g_no_terminal_output) {
		return;
	}
	if (strlen(text) > width) {
		return;
	}

	int new_width = width - strlen(text);
	int side	  = new_width >> 1;
	for (int i = 0; i <= width; i++) {
		if (i >= side && i <= side + strlen(text)) {
			PRINT("%s", text);
			i += strlen(text);
		} else {
			PRINT("%c", padding);
		}
	}
	PRINT("\n");
}
