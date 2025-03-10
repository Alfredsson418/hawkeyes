#include "../../include/ui/display_file.h"

void print_file(char *file, int width) {
	FILE *file_ptr = fopen(file, "r");
	if (file_ptr == NULL) {
		ERR_PRINT("Error opening file: %s", file);
		return;
	}

	char buffer[width];
	while (fgets(buffer, width, file_ptr)) {
		PRINT("%s", buffer);
	}
	PRINT("\n");

	fclose(file_ptr);
}
