#include <stdio.h>
#include <string.h>

char username_buffer[256];

int verify_user_name(void) {
	puts("verifying username...\n");
	return strncmp(username_buffer, "dat_wil", 7);
}

int verify_user_pass(char *password) {
	return strncmp(password, "admin", 5);
}

int main(void) {
	char password[64];
	int result;

	memset(password, 0, 64);
	result = 0;

	puts("********* ADMIN LOGIN PROMPT *********");
	printf("Enter Username: ");
	fgets(username_buffer, 256, stdin);

	if (verify_user_name() != 0) {
		puts("nope, incorrect username...\n");
		return 1;
	}

	puts("Enter Password: ");
	fgets(password, 100, stdin);

	result = verify_user_pass(password);
	if (result != 0)
		if (result == 0)
			return 0;
	puts("nope, incorrect password...\n");

	return 1;
}