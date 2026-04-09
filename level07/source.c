#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void	clear_stdin(void) {
	char	c = '\0';

	while (1) {
		c = getchar();
		if (c == '\n' || c == (char)0xff)
			break ;
	}
}

unsigned int	get_unum(void) {
	unsigned int	num = 0;

	fflush(stdout);
	scanf("%u", &num);
	clear_stdin();
	return (num);
}

unsigned int	store_number(unsigned int *data) {
	unsigned int	number = 0;
	unsigned int	index = 0;

	printf(" Number: ");
	number = get_unum();
	printf(" Index: ");
	index = get_unum();

	if (index % 3 == 0 || (number >> 24) == 0xb7) {
		puts(" *** ERROR! ***");
		puts("   This index is reserved for wil!");
		puts(" *** ERROR! ***");
		return (1);
	}

	data[index] = number;
	return (0);
}

unsigned int	read_number(unsigned int *data) {
	unsigned int	index = 0;

	printf(" Index: ");
	index = get_unum();
	printf(" Number at data[%u] is %u\n", index, data[index]);
	return (0);
}

int	main(int argc, char **argv, char **envp) {
	int				ret;
	char			cmd[20];
	unsigned int	data[100];

	memset(cmd, 0, 20);
	memset(data, 0, sizeof(data));

	while (*argv) {
		memset(*argv, 0, strlen(*argv));
		argv++;
	}

	while (*envp) {
		memset(*envp, 0, strlen(*envp));
		envp++;
	}

	puts("----------------------------------------------------\n"
		 "  Welcome to wil's crappy number storage service!   \n"
		 "----------------------------------------------------\n"
		 " Commands:                                          \n"
		 "    store - store a number into the data storage    \n"
		 "    read  - read a number from the data storage     \n"
		 "    quit  - exit the program                        \n"
		 "----------------------------------------------------\n"
		 "   wil has reserved some storage :>                 \n"
		 "----------------------------------------------------\n");

	while (1) {
		printf("Input command: ");
		ret = 1;
		fgets(cmd, 20, stdin);
		cmd[strlen(cmd) - 1] = '\0';

		if (strncmp(cmd, "store", 5) == 0)
			ret = store_number(data);
		else if (strncmp(cmd, "read", 4) == 0)
			ret = read_number(data);
		else if (strncmp(cmd, "quit", 4) == 0)
			break ;

		if (ret)
			printf(" Failed to do %s command\n", cmd);
		else
			printf(" Completed %s command successfully\n", cmd);

		memset(cmd, 0, 20);
	}

	return (0);
}
