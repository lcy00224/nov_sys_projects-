#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_SIZE 5

int buffer = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t notfull = PTHREAD_COND_INITIALIZER;
pthread_cond_t notempty = PTHREAD_COND_INITIALIZER;

void* producer(void* arg) {
    for (int i = 0; i<10; i++) {
        pthread_mutex_lock(&lock);
        while (buffer == MAX_SIZE) {
            printf("生产者：仓库满了，停止生产\n");
            pthread_cond_wait(&notfull, &lock);
        }
        buffer++;
        printf("生产者：生产了一个商品，仓储：%d\n",buffer);
        pthread_cond_signal(&notempty);
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}
void* consumer(void* arg) {
    for (int i = 0; i<10; i++) {
        pthread_mutex_lock(&lock);
        while (buffer == 0) {
            printf("消费者：没货了，停止消费\n");
            pthread_cond_wait(&notempty, &lock);
        }
        buffer--;
        printf("消费者：购买了一个商品，余量：%d\n",buffer);
        pthread_cond_signal(&notfull);
        pthread_mutex_unlock(&lock);
        sleep(2);
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, producer, NULL);
    pthread_create(&t2, NULL, consumer, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&notfull);
    pthread_cond_destroy(&notempty);
    return 0;
}

