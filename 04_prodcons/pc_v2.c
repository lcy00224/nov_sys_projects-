#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_SIZE 5
int buffer = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t notfull = PTHREAD_COND_INITIALIZER;
pthread_cond_t notempt = PTHREAD_COND_INITIALIZER;

typedef struct {
    int id;
} ThreadArg;

void* producer(void *arg) {
    ThreadArg* p  = (ThreadArg*)arg;
    for (int i=0; i<15; i++) {
        pthread_mutex_lock(&lock);
        while (buffer == MAX_SIZE) {
            pthread_cond_wait(&notfull, &lock);
        }
        buffer ++;
        printf("生产者%d生产了一个产品，现存%d个产品\n", p->id, buffer);
        pthread_cond_signal(&notempt);
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}
void* consumer(void *arg) {
    ThreadArg* p = (ThreadArg*)arg;
    for (int i=0; i<10 ;i++) {
        pthread_mutex_lock(&lock);
        while (buffer == 0) {
            pthread_cond_wait(&notempt, &lock);
        }
        buffer--;
        printf("消费者%d购买了一个产品，现存%d个产品\n", p->id, buffer);
        pthread_cond_signal(&notfull);
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main() {
    pthread_t prod[2];
    pthread_t cons[3];
    ThreadArg p[2];
    ThreadArg c[3];
    for (int i =0; i<2; i++) {
        p[i].id = i;
        pthread_create(&prod[i], NULL, producer, &p[i]);
    }
    for (int i = 0; i<3; i++) {
        c[i].id = i;
        pthread_create(&cons[i], NULL, consumer, &c[i]);
    }
    for (int i =0; i<2; i++) {
        
        pthread_join(prod[i], NULL);
    }
    for (int i = 0; i<3; i++) {
        pthread_join(cons[i], NULL);
    }
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&notfull);
    pthread_cond_destroy(&notempt);
    return 0;
}
