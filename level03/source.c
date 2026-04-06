#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void decrypt(int key) {
	char	encrypted[] = "\x51\x7d\x7c\x75\x60\x73\x66\x67\x7e\x73\x66\x7b\x7d\x7c\x61\x33";
	int		len = strlen(encrypted);
	int		i = 0;

	while (i < len) {
		encrypted[i] = encrypted[i] ^ key;
		i++;
	}

	if (strncmp(encrypted, "Congratulations!", 17) == 0)
		system("/bin/sh");
	else
		puts("\nInvalid Password");
}

void test(int input, int reference) {
	int	diff = reference - input;

	switch (diff) {
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
			decrypt(diff);
			break ;
		default:
			decrypt(rand());
			break ;
	}
}

int main(void) {
	int	password;

	srand(time(0));

	puts("***********************************");
	puts("*\t\tlevel03\t\t**");
	puts("***********************************");
	printf("Password:");
	scanf("%d", &password);

	test(password, 0x1337d00d);

	return 0;
}