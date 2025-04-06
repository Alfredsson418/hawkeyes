#include "../../include/other/service_parser.h"

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
	int	 file_size = strlen(SERVICE_STORAGE) + strlen(protocol);
	char file[file_size];
	memset(file, 0, file_size);

	sprintf(file, "%s%s", SERVICE_STORAGE, protocol);

	FILE *temp = fopen(file, "a");
	if (temp) {
		fprintf(temp, "%s,%s\n", port, service);
		fclose(temp);
	}

	return 0;
}

int setup_services(bool force) {

	if (mkdir(TMP_STORAGE, 0777) < 0) {
	}

	if (mkdir(SERVICE_STORAGE, 0777) < 0) {
	}

	if (force) {
		/*
		TODO
		Remove files from dir
		*/
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
