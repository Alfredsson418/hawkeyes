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

void print_json(char *file, int width, char *key) {
	FILE *file_ptr = fopen(file, "r");
	if (file_ptr == NULL) {
		ERR_PRINT("Error opening file: %s", file);
		return;
	}

	char buffer[1024];
	int	 len = fread(buffer, 1, sizeof(buffer), file_ptr);
	fclose(file_ptr);

	cJSON *json = cJSON_Parse(buffer);

	if (json == NULL) {
		ERR_PRINT("cJSON failed to parse file\n");
		cJSON_free(json);
		return;
	}

	cJSON *string = cJSON_GetObjectItemCaseSensitive(json, key);

	if (cJSON_IsString(string)) {
		PRINT("%s\n", string->valuestring);
	}
	cJSON_Delete(json);
}
