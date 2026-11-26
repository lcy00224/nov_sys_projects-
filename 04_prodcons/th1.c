#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
 
void* pthread_func(void* arg) {
    int* num = (int*)arg;
    printf("我是子线程，我收到的参数是：%d\n",*num);
    sleep(1);
    printf("子线程即将退出\n");
    return NULL;
}

int main() {
    pthread_t tid;
    int x = 42;
    printf("主线程开始，准备创建子线程\n");
    pthread_create(&tid,NULL, pthread_func, &x);
    printf("等待子线程结束...\n");
    pthread_join(tid, NULL);
    printf("主线程结束\n");
    return 0;
}
