#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct	s_msg {
	char		msg[140];
	char		username[40];
	int			msg_len;
}				t_msg;

void	secret_backdoor(void) {
	char	buffer[128];

	fgets(buffer, 128, stdin);
	system(buffer);
}

void set_username(t_msg *data) {
	char	tmp[128];
	int		i;

	memset(tmp, 0, 128);
	puts(">: Enter your username");
	printf(">>: ");
	fgets(tmp, 128, stdin);

	i = 0;
	while (i <= 40 && tmp[i]) {
		data->username[i] = tmp[i];
		i++;
	}

	printf(">: Welcome, %s", data->username);
}

void set_msg(t_msg *data) {
	char	tmp[1024];

	memset(tmp, 0, 1024);
	puts(">: Msg @Unix-Dude");
	printf(">>: ");
	fgets(tmp, 1024, stdin);
	strncpy(data->msg, tmp, data->msg_len);
}

void handle_msg(void) {
	t_msg	data;

	memset(data.username, 0, 40);
	data.msg_len = 140;

	set_username(&data);
	set_msg(&data);

	puts(">: Msg sent!");
}

int	main(void) {
	puts("--------------------------------------------\n"
		 "|   ~Welcome to l33t-m$n ~   v1337        |\n"
		 "--------------------------------------------");
	handle_msg();
	return 0;
}