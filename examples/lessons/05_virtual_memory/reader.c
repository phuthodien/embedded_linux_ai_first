#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    // Open existing shared memory
    int shm_fd = shm_open("/demo_shm", O_RDWR, 0666);
    if (shm_fd == -1) {
        printf("Reader: Shared memory not found. Run writer first!\n");
        return 1;
    }
    
    // Map into process
    char *shared_data = mmap(NULL, 1024, PROT_READ | PROT_WRITE,
                            MAP_SHARED, shm_fd, 0);
    
    // Read data
    printf("Reader: Data from shared memory: %s\n", shared_data);
    printf("Reader PID: %d\n", getpid());
    
    // Modify data
    strcat(shared_data, " + Modified by reader!");
    printf("Reader: Modified shared data\n");
    
    munmap(shared_data, 1024);
    close(shm_fd);
    return 0;
}