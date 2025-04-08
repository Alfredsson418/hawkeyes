#include "../../include/other/service_parser.h"

/*
https://codereview.stackexchange.com/questions/263536/c-delete-files-and-directories-recursively-if-directory
*/
int _remove_recurv(const char *path_to_dir) {
	DIR *dir = opendir(path_to_dir);

	if (dir) {
		struct dirent *entry;
		while ((entry = readdir(dir))) {
			if (!strcmp(".", entry->d_name) || !strcmp("..", entry->d_name)) {
				continue;
			}
			char filename[strlen(path_to_dir) + strlen(entry->d_name) + 2];
			sprintf(filename, "%s/%s", path_to_dir, entry->d_name);
			int (*const remove_func)(const char *) =
				entry->d_type == DT_DIR ? _remove_recurv : remove;
			if (remove_func(filename)) {
				closedir(dir);
				return -1;
			}
		}
		if (closedir(dir)) {
			return -1;
		}
	}

	return remove(path_to_dir);
}

/*
	Checks if we have already have parsed the ports
	by seeing if the SERVICE_CHECK file exists
*/
int _skip() {
	FILE *check = fopen(SERVICE_CHECK, "r");
	if (check == NULL) {
		FILE *check = fopen(SERVICE_CHECK, "w");
		fclose(check);
		return 0;
	}
	fclose(check);
	return 1;
}

int _append_to_file(char *protocol, char *service, char *port) {
	int	 file_size = strlen(SERVICE_STORAGE) + strlen(protocol) + 2;
	char file[file_size];
	memset(file, 0, file_size);

	sprintf(file, "%s/%s", SERVICE_STORAGE, protocol);

	FILE *temp = fopen(file, "a");
	if (temp) {
		fprintf(temp, "%s,%s\n", port, service);
		fclose(temp);
	}

	return 0;
}

int setup_services(bool force) {

	if (force) {
		if (_remove_recurv(SERVICE_STORAGE) < 0) {
			ERR_PRINT("Failed to delete service path, may not exist when this "
					  "runs!\n");
		}
	}

	if (mkdir(TMP_STORAGE, 0777) < 0) {
	}

	if (mkdir(SERVICE_STORAGE, 0777) < 0) {
	}

	if (_skip()) {
		return 1;
	}

	FILE *s_file = fopen(SERVICES, "r");

	char  line[200];
	char *service_name, *port_protocol, *port, *protocol;
	char *saveptr1, *saveptr2;

	while (fgets(line, 200, s_file) != NULL) {
		if (line[0] == '#' || line[0] == '\n' || line[0] == ' ') {
			continue;
		}

		// Get service name
		service_name = strtok_r(line, " ", &saveptr1);
		if (!service_name)
			continue;

		// Get port/protocol combination
		port_protocol = strtok_r(NULL, " ", &saveptr1);
		if (!port_protocol)
			continue;

		// Split port and protocol
		port	 = strtok_r(port_protocol, "/", &saveptr2);
		protocol = strtok_r(NULL, "/", &saveptr2);

		if (port && protocol) {
			// Remove trailing newline if present
			char *newline = strchr(protocol, '\n');
			if (newline) {
				*newline = '\0';
			}

			_append_to_file(protocol, service_name, port);
		}
	}
	fclose(s_file);

	return 0;
}
