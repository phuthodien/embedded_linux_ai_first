/*
 * test_echo.c - Test program for echo_misc character driver
 *
 * Tests open, write, read, close on /dev/echo_misc.
 * Run on BeagleBone Black after insmod echo_misc.ko.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define DEVICE_PATH	"/dev/echo_misc"
#define BUF_SIZE	256

int main(void)
{
	int fd;
	char write_buf[] = "Hello from test_echo!";
	char read_buf[BUF_SIZE];
	ssize_t ret;

	/* Step 1: open */
	printf("=== Step 1: open(%s) ===\n", DEVICE_PATH);
	fd = open(DEVICE_PATH, O_RDWR);
	if (fd < 0) {
		perror("open");
		return EXIT_FAILURE;
	}
	printf("fd = %d\n\n", fd);

	/* Step 2: write */
	printf("=== Step 2: write \"%s\" ===\n", write_buf);
	ret = write(fd, write_buf, strlen(write_buf));
	if (ret < 0) {
		perror("write");
		close(fd);
		return EXIT_FAILURE;
	}
	printf("wrote %zd bytes\n\n", ret);

	/* Step 3: seek back to beginning before read */
	lseek(fd, 0, SEEK_SET);

	/* Step 4: read */
	printf("=== Step 3: read ===\n");
	memset(read_buf, 0, sizeof(read_buf));
	ret = read(fd, read_buf, sizeof(read_buf));
	if (ret < 0) {
		perror("read");
		close(fd);
		return EXIT_FAILURE;
	}
	printf("read %zd bytes: \"%s\"\n\n", ret, read_buf);

	/* Step 5: close (triggers release in driver) */
	printf("=== Step 4: close ===\n");
	close(fd);
	printf("closed fd %d\n", fd);

	/* Verify: data matches? */
	if (ret == (ssize_t)strlen(write_buf) &&
	    memcmp(read_buf, write_buf, ret) == 0)
		printf("\nPASS: read back matches written data\n");
	else
		printf("\nFAIL: data mismatch!\n");

	return EXIT_SUCCESS;
}
