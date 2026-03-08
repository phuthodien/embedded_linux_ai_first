/*
 * env_path_simple.c — Simplest demo: PATH affects execlp()
 *
 * Run 1: execlp("echo") with normal PATH   => OK
 * Run 2: execlp("echo") after clearing PATH => FAIL
 *
 * Compile:
 *   gcc -g -O0 -Wall -Wextra -std=c99 -o env_path_simple env_path_simple.c
 */

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
	pid_t pid;
	int status;

	/* --- Run 1: Normal PATH --- */
	printf("=== Run 1: PATH binh thuong ===\n");
	fflush(stdout);

	pid = fork();
	if (pid == 0) {
		printf("PATH = \"%s\"\n", getenv("PATH"));
		execlp("echo", "echo", "Hello from child!", NULL);
		perror("execlp");
		exit(1);
	}
	waitpid(pid, &status, 0);
	printf("=> exit code: %d\n\n", WEXITSTATUS(status));

	/* --- Run 2: Clear PATH --- */
	printf("=== Run 2: Xoa PATH ===\n");
	setenv("PATH", "", 1);
	printf("PATH = \"%s\"\n", getenv("PATH"));
	fflush(stdout);

	pid = fork();
	if (pid == 0) {
		execlp("echo", "echo", "Hello from child!", NULL);
		perror("execlp");
		exit(1);
	}
	waitpid(pid, &status, 0);
	printf("=> exit code: %d\n", WEXITSTATUS(status));

	return 0;
}
