/*
 * fork_exec_example.c - Demo fork() + exec() combined
 *
 * Parent forks a child, child uses execvp() to run "ls -l ./",
 * parent waits for child to finish, then continues its own work.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
	pid_t pid;
	int status;

	printf("Parent PID = %d, about to fork...\n", getpid());

	pid = fork();
	if (pid < 0) {
		perror("fork");
		exit(EXIT_FAILURE);
	}

	if (pid == 0) {
		/* Child: replace itself with "ls -l ./" */
		char *args[] = {"ls", "-l", "./", NULL};

		printf("Child PID = %d, calling execvp(\"ls\")...\n",
		       getpid());
		execvp("ls", args);

		/* only reached if execvp() fails */
		perror("execvp");
		_exit(EXIT_FAILURE);
	}

	/* Parent: wait for child to finish */
	if (waitpid(pid, &status, 0) == -1) {
		perror("waitpid");
		exit(EXIT_FAILURE);
	}

	if (WIFEXITED(status))
		printf("\nParent: child exited with status %d\n",
		       WEXITSTATUS(status));

	printf("Parent: I'm still running! PID = %d\n", getpid());

	return 0;
}
