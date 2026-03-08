#include <stdio.h>
#include <unistd.h>

int main() {
    int x = 10;
    pid_t pid = fork();

    if (pid > 0) {
        x = 20;  // Cha thay đổi x → kernel tạo copy riêng
        printf("Cha: x = %d, PID = %d\n", x, getpid());
    } else if (pid == 0) {
        printf("Con: x = %d, PID = %d\n", x, getpid());
        // Con thấy x = 10, không bị ảnh hưởng bởi cha
    }
    return 0;
}