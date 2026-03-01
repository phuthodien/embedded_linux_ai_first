#define _DEFAULT_SOURCE /* expose sync() prototype with -std=c99 */

/*
 * file_io_demo.c - Demonstrate basic file I/O system calls:
 *   open(), close(), read(), write(), lseek(), sync()
 *
 * This program creates a file, writes data to it, seeks back to the
 * beginning, reads the data back, then modifies part of the file
 * using lseek() to reposition the file offset.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define FILE_PATH	"testfile.txt"
#define BUF_SIZE	256

int main(void)
{
	int fd;
	ssize_t num_written, num_read;
	off_t pos;
	char write_buf[] = "Hello, this is a file I/O demo using system calls!\n";
	char extra_buf[] = "[INSERTED]";
	char read_buf[BUF_SIZE];

	/* 1. open() - Create file, truncate if exists */
	fd = open(FILE_PATH, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (fd == -1) {
		perror("open");
		exit(EXIT_FAILURE);
	}
	printf("1. open(): opened '%s', fd = %d\n", FILE_PATH, fd);

	/* 2. write() - Write initial data to the file */
	num_written = write(fd, write_buf, strlen(write_buf));
	if (num_written == -1) {
		perror("write");
		close(fd);
		exit(EXIT_FAILURE);
	}
	printf("2. write(): wrote %zd bytes\n", num_written);

	/* 3. sync() - Flush all filesystem caches to disk */
	sync();
	printf("3. sync(): flushed filesystem buffers to disk\n");

	/* 4. lseek() - Seek back to the beginning of the file */
	pos = lseek(fd, 0, SEEK_SET);
	if (pos == -1) {
		perror("lseek SEEK_SET");
		close(fd);
		exit(EXIT_FAILURE);
	}
	printf("4. lseek(SEEK_SET, 0): file offset = %ld\n", (long)pos);

	/* 5. read() - Read back what we wrote */
	num_read = read(fd, read_buf, BUF_SIZE - 1);
	if (num_read == -1) {
		perror("read");
		close(fd);
		exit(EXIT_FAILURE);
	}
	read_buf[num_read] = '\0';
	printf("5. read(): read %zd bytes: \"%s\"\n", num_read, read_buf);

	/* 6. lseek() - Seek to offset 7 ("this is a file...") to overwrite */
	pos = lseek(fd, 7, SEEK_SET);
	if (pos == -1) {
		perror("lseek offset 7");
		close(fd);
		exit(EXIT_FAILURE);
	}
	printf("6. lseek(SEEK_SET, 7): file offset = %ld\n", (long)pos);

	/* 7. write() - Overwrite part of the content */
	num_written = write(fd, extra_buf, strlen(extra_buf));
	if (num_written == -1) {
		perror("write extra");
		close(fd);
		exit(EXIT_FAILURE);
	}
	printf("7. write(): overwrote %zd bytes at offset 7\n", num_written);

	/* 8. lseek() - Use SEEK_END to get total file size */
	pos = lseek(fd, 0, SEEK_END);
	if (pos == -1) {
		perror("lseek SEEK_END");
		close(fd);
		exit(EXIT_FAILURE);
	}
	printf("8. lseek(SEEK_END, 0): file size = %ld bytes\n", (long)pos);

	/* 9. Read the final content from the beginning */
	lseek(fd, 0, SEEK_SET);
	num_read = read(fd, read_buf, BUF_SIZE - 1);
	if (num_read == -1) {
		perror("read final");
		close(fd);
		exit(EXIT_FAILURE);
	}
	read_buf[num_read] = '\0';
	printf("9. Final content: \"%s\"\n", read_buf);

	/* 10. close() - Close the file descriptor */
	if (close(fd) == -1) {
		perror("close");
		exit(EXIT_FAILURE);
	}
	printf("10. close(): fd %d closed\n", fd);

	printf("\nDone! Check '%s' for the result.\n", FILE_PATH);
	return 0;
}
