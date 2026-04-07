#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/ptrace.h>

int	auth(char *login, unsigned int serial) {
	unsigned int	i;
	unsigned int	hash;
	unsigned int	len;

	login[strcspn(login, "\n")] = '\0';

	len = strnlen(login, 32);

	if (len <= 5)
		return (1);

	if (ptrace(PTRACE_TRACEME, 0, 1, 0) == -1) {
		puts("TAMPERING DETECTED");
		return (1);
	}

	hash = (login[3] ^ 4919) + 6221293;

	for (i = 0; i < len; i++) {
		if (login[i] <= 31) return (1);

		unsigned int	tmp = login[i] ^ hash;
		hash += tmp - ((tmp - (tmp * 2284010283 >> 32) >> 1) + (tmp * 2284010283 >> 32) >> 10) * 1337;
	}

	return (hash == serial) ? 0 : 1;
}

int	main(void) {
	char			login[32];
	unsigned int	serial;

	puts("***********************************");
	puts("*\t\tlevel06\t\t  *");
	puts("***********************************");

	printf("-> Enter Login: ");
	fgets(login, sizeof(login), stdin);

	puts("***********************************");
	puts("***** NEW ACCOUNT DETECTED ********");
	puts("***********************************");

	printf("-> Enter Serial: ");
	scanf("%u", &serial);

	if (auth(login, serial) != 0)
		return (1);

	puts("Authenticated!");
	system("/bin/sh");
	return (0);
}