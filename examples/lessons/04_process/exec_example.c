/*
 * exec_example.c - Demo execvp() to replace current process
 *
 * After execvp(), this process is completely replaced by "ls -l /tmp".
 * Code after execvp() only runs if exec fails.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	char *args[] = {"ls", "-l", "./", NULL};

	printf("PID = %d, about to exec \"ls -l ./\"...\n", getpid());

	execvp("ls", args);

	/* only reached if execvp() fails */
	printf("hello world\n");
	return EXIT_FAILURE;
}
