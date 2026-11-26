#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t notfull = PTHREAD_COND_INITIALIZER;
pthread_cond_t notempty = PTHREAD_COND_INITIALIZER;

int buffer = 0;
typedef struct {
    int id;
} PthreadArgs;
void* producer(void* p) {
    PthreadArgs* arg = (PthreadArgs*)p;
    for (int i = 0; i<5; i++) {
        pthread_mutex_lock(&lock);
        while (buffer == 5){
            pthread_cond_wait(&notfull, &lock);
        }
        buffer++;
        printf("生产者%d 生产了一个产品，现存%d\n", arg->id, buffer);
        pthread_cond_signal(&notempty);
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}
void* consumer(void* p) {
    PthreadArgs* arg = (PthreadArgs*)p;
    for (int i = 0; i<3; i++) {
        pthread_mutex_lock(&lock);
        while (buffer == 0){
            pthread_cond_wait(&notempty, &lock);
        }
        buffer--;
        printf("消费者者%d 消费了一个产品，现存%d\n", arg->id, buffer);
        pthread_cond_signal(&notfull);
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main() {
    PthreadArgs p[2], c[3];
    pthread_t pthread_pc[5];
    for (int i = 0; i < 5; i++){
        if (i < 2){
            p[i].id = i;
            pthread_create(&pthread_pc[i], NULL, producer, &p[i]);
        } else{
            c[i-2].id = i;
            pthread_create(&pthread_pc[i], NULL, consumer, &c[i-2]);
        }
    }
    for (int i = 0; i < 5; i++){
        pthread_join(pthread_pc[i], NULL);
    }
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&notfull);
    pthread_cond_destroy(&notempty);
    return 0;
}
