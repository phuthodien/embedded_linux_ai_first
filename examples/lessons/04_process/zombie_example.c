#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();
    if (pid == 0) {
        // Process con: kết thúc ngay
        printf("Con (PID %d) kết thúc\n", getpid());
        exit(0);
    }
    // Process cha: không gọi wait(), ngủ 30 giây
    printf("Cha (PID %d) đang ngủ, con đã thành zombie\n", getpid());
    sleep(30);
    return 0;
}