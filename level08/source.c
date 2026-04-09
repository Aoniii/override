#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

void	log_wrapper(FILE *logfile, char *msg, char *filename) {
	char	buffer[256];

	strcpy(buffer, msg);
	snprintf(buffer + strlen(buffer), 254 - strlen(buffer), filename);
	buffer[strcspn(buffer, "\n")] = '\0';
	fprintf(logfile, "LOG: %s\n", buffer);
}

int	main(int argc, char **argv) {
	char		c = (char)0xff;
	int			fd = -1;
	FILE		*logfile;
	FILE		*src;
	char		dest_path[100];

	if (argc != 2)
		printf("Usage: %s filename\n", argv[0]);

	logfile = fopen("./backups/.log", "w");
	if (logfile == NULL) {
		printf("ERROR: Failed to open %s\n", "./backups/.log");
		exit(1);
	}

	log_wrapper(logfile, "Starting back up: ", argv[1]);

	src = fopen(argv[1], "r");
	if (src == NULL) {
		printf("ERROR: Failed to open %s\n", argv[1]);
		exit(1);
	}

	strcpy(dest_path, "./backups/");
	strncat(dest_path, argv[1], 99 - strlen(dest_path));

	fd = open(dest_path, O_WRONLY | O_CREAT, 0660);
	if (fd < 0) {
		printf("ERROR: Failed to open %s%s\n", "./backups/", argv[1]);
		exit(1);
	}

	while (1) {
		c = fgetc(src);
		if (c == (char)0xff)
			break ;
		write(fd, &c, 1);
	}

	log_wrapper(logfile, "Finished back up ", argv[1]);

	fclose(src);
	close(fd);

	return {0};
}