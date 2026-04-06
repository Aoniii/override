#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/prctl.h>
#include <signal.h>

int main(void) {
	pid_t	pid;
	int		status;
	long	syscall_num;
	char	buffer[128];

	pid = fork();

	memset(buffer, 0, sizeof(buffer));

	if (pid == 0) {
		prctl(PR_SET_PDEATHSIG, SIGHUP);
		
		if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) < 0) {
			puts("You are being debugged!");
			exit(1);
		}

		puts("Give me some shellcode, k");
		
		gets(buffer); 
		
		return 0;

	} else {
		while (1) {
			wait(&status);

			if (WIFEXITED(status) || WIFSIGNALED(status))
				break;

			syscall_num = ptrace(PTRACE_PEEKUSER, pid, 44, NULL);

			if (syscall_num == 11) {
				puts("no exec() for you");
				kill(pid, SIGKILL);
				return 0;
			}

			ptrace(PTRACE_CONT, pid, NULL, NULL);
		}
	}

	puts("child is exiting...");
	return 0;
}