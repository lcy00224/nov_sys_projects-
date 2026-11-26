#include <stdio.h>
#include <pthread.h>

int global_count = 0;
void* add_func(void* arg) {
    for (int i = 0; i<10000; i++) {
        global_count++;
    }
    return NULL;
}
int main() {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, add_func, NULL);
    pthread_create(&t2, NULL, add_func, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("global_count:%d\n",global_count);
    return 0;
}
