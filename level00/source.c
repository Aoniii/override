#include <stdio.h>

int main(void) {
	int	password;

    puts("***********************************");
    puts("*        -Level00 -               *");
    puts("***********************************");

    printf("Password:");
	scanf("%d", &password);

    if (password != 5276) {
        puts("\nInvalid Password!");
        return (1);
    }

    puts("\nAuthenticated!");
    system("/bin/sh");
    return (0);
}
