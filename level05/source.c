#include <stdio.h>
#include <stdlib.h>

int main(void) {
	char			buf[100];
	unsigned int	i = 0;
	unsigned int	len = 0;

	fgets(buf, 100, stdin);

	while (buf[len]) len++;

	while (i < len) {
		if (buf[i] > 0x40 && buf[i] <= 0x5A)
			buf[i] ^= 0x20;
		i++;
	}

	printf(buf);
	exit(0);
	return (0);
}
