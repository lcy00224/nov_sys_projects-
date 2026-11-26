#include <stdio.h>
#include <pthread.h>

int global_count = 0; // 共享变量
pthread_mutex_t lock;
void* add_func(void* arg) {
    for (int i = 0; i < 100000; i++) {
        pthread_mutex_lock(&lock);
        global_count++; // 核心危险动作！
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main() {
    pthread_mutex_init(&lock, NULL);
    pthread_t t1, t2;
    pthread_create(&t1, NULL, add_func, NULL);
    pthread_create(&t2, NULL, add_func, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_mutex_destroy(&lock);
    // 预期结果是 200000
    printf("最终 global_count 的值是: %d\n", global_count);
    return 0;
}
