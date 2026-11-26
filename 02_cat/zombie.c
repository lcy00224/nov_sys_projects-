#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main() {
    pid_t pid = fork();
    if (pid == 0){
        printf("子进程PID：%d\n",getpid());
        exit(0);
    }else {
        printf("父进程在睡觉PID:%d,不去回收子进程：%d\n",getpid(),pid);
        sleep(20);
    }
    return 0;
}
