#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t lock_a = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_b = PTHREAD_MUTEX_INITIALIZER;

void *thread_1(void *arg) {
    pthread_mutex_lock(&lock_a);
    printf("Thread 1 lấy được lock_a, đang chờ lock_b...\n");
    sleep(1);                          // nhường thời gian để thread 2 kịp lock_b
    pthread_mutex_lock(&lock_b);       // ← chờ mãi mãi
    printf("Thread 1 done\n");         // không bao giờ in được dòng này
    pthread_mutex_unlock(&lock_b);
    pthread_mutex_unlock(&lock_a);
    return NULL;
}

void *thread_2(void *arg) {
    pthread_mutex_lock(&lock_b);
    printf("Thread 2 lấy được lock_b, đang chờ lock_a...\n");
    sleep(1);
    pthread_mutex_lock(&lock_a);       // ← chờ mãi mãi
    printf("Thread 2 done\n");         // không bao giờ in được dòng này
    pthread_mutex_unlock(&lock_a);
    pthread_mutex_unlock(&lock_b);
    return NULL;
}

int main() {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, thread_1, NULL);
    pthread_create(&t2, NULL, thread_2, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);            // main treo tại đây mãi mãi
    return 0;
}