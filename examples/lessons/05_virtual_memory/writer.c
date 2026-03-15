#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main() {
    // Create shared memory
    int shm_fd = shm_open("/demo_shm", O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, 1024);
    
    // Map into process
    char *shared_data = mmap(NULL, 1024, PROT_READ | PROT_WRITE, 
                            MAP_SHARED, shm_fd, 0);
    
    // Write data
    strcpy(shared_data, "Message from writer process!");
    printf("Writer: Data written to shared memory\n");
    printf("Writer PID: %d\n", getpid());
    
    printf("Press Enter to continue...\n");
    getchar();
    
    // Cleanup
    munmap(shared_data, 1024);
    shm_unlink("/demo_shm");
    return 0;
}