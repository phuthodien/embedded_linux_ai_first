/*
 * writer.c - Shared Memory + Semaphore (simple example)
 *
 * Writes a counter into POSIX shared memory every 1 second.
 * Uses a named semaphore to prevent reader from accessing
 * shared memory while writer is updating.
 *
 * Build:  gcc -g -O0 -Wall -Wextra -std=c99 -o writer writer.c -lrt -lpthread
 * Run:    ./writer       (start before reader)
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
	int fd, count = 0;
	int *ptr;
	sem_t *sem;

	/* Create shared memory */
	fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
	if (fd < 0) {
		perror("shm_open");
		return 1;
	}
	ftruncate(fd, SHM_SIZE);

	/* Map into this process's address space */
	ptr = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE,
		   MAP_SHARED, fd, 0);
	close(fd);
	if (ptr == MAP_FAILED) {
		perror("mmap");
		return 1;
	}

	/* Create named semaphore, initial value = 1 (unlocked) */
	sem = sem_open(SEM_NAME, O_CREAT, 0666, 1);
	if (sem == SEM_FAILED) {
		perror("sem_open");
		return 1;
	}

	printf("Writer started. Press Ctrl+C to stop.\n");

	while (1) {
		sem_wait(sem);		/* lock */
		*ptr = ++count;		/* write to shared memory */
		printf("[Writer] wrote: %d\n", *ptr);
		sem_post(sem);		/* unlock */

		sleep(1);
	}

	/* Cleanup (reached if you add signal handling) */
	munmap(ptr, SHM_SIZE);
	shm_unlink(SHM_NAME);
	sem_close(sem);
	sem_unlink(SEM_NAME);

	return 0;
}
