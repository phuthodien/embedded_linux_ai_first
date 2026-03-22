/*
 * reader.c - Shared Memory + Semaphore (simple example)
 *
 * Reads a counter from POSIX shared memory written by writer.c.
 * Uses a named semaphore to avoid reading while writer is updating.
 *
 * Build:  gcc -g -O0 -Wall -Wextra -std=c99 -o reader reader.c -lrt -lpthread
 * Run:    ./reader       (start writer first)
 */

#define _POSIX_C_SOURCE 200809L
#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>

#define SHM_NAME "/my_shm"
#define SEM_NAME "/my_sem"
#define SHM_SIZE sizeof(int)

int main(void)
{
	int fd;
	int *ptr;
	sem_t *sem;

	/* Open existing shared memory (writer creates it) */
	fd = shm_open(SHM_NAME, O_RDONLY, 0666);
	if (fd < 0) {
		perror("shm_open (is writer running?)");
		return 1;
	}

	/* Map as read-only */
	ptr = mmap(NULL, SHM_SIZE, PROT_READ, MAP_SHARED, fd, 0);
	close(fd);
	if (ptr == MAP_FAILED) {
		perror("mmap");
		return 1;
	}

	/* Open existing named semaphore (writer creates it) */
	sem = sem_open(SEM_NAME, 0);
	if (sem == SEM_FAILED) {
		perror("sem_open (is writer running?)");
		return 1;
	}

	printf("Reader started. Press Ctrl+C to stop.\n");

	while (1) {
		sem_wait(sem);		/* lock */
		printf("[Reader] read: %d\n", *ptr);
		sem_post(sem);		/* unlock */

		sleep(1);
	}

	/* Cleanup */
	munmap(ptr, SHM_SIZE);
	sem_close(sem);

	return 0;
}
