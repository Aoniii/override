#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
	char	password_from_file[41];	// rbp-0xa0 (41 bytes = 0x29)
	char	username[100];			// rbp-0x70
	char	user_password[100];		// rbp-0x110
	FILE	*fp;					// rbp-0x8
	int		bytes_read;				// rbp-0xc

	memset(username, 0, 100);
	memset(password_from_file, 0, 41);
	memset(user_password, 0, 100);
	fp = NULL;
	bytes_read = 0;

	fp = fopen("/home/users/level03/.pass", "r");
	if (fp == NULL) {
		fwrite("ERROR: failed to open password file\n", 1, 36, stderr);
		exit(1);
	}

	bytes_read = fread(password_from_file, 1, 41, fp);
	password_from_file[strcspn(password_from_file, "\n")] = '\0';

	if (bytes_read != 41) {
		fwrite("ERROR: failed to read password file\n", 1, 36, stderr);
		exit(1);
	}

	fclose(fp);

	puts("===== [ Secure Access System v1.0 ] =====");
	puts("/***************************************\\");
	puts("| You must login to access this system. |");
	puts("\\***************************************/");

	printf("--[ Username: ");
	fgets(username, 100, stdin);
	username[strcspn(username, "\n")] = '\0';

	printf("--[ Password: ");
	fgets(user_password, 100, stdin);
	user_password[strcspn(user_password, "\n")] = '\0';

	puts("*****************************************");

	if (strncmp(password_from_file, user_password, 41) == 0) {
		printf("Greetings, %s!\n", username);
		system("/bin/sh");
		return 0;
	} else {
		printf(username);
		puts(" does not have access!");
		exit(1);
	}
}