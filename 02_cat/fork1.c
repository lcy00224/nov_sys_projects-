#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
int main() {
    printf("--- fork 之前, 只有一个进程 ---");
    printf("我的PID是: %d\n",getpid());
    pid_t pid = fork();
    //从这开始代码会被两个进程（父子）同时执行
    if (pid < 0) {
        perror("fork()失败");
    }else if (pid == 0) {
        //这里的代码只会被子进程执行
        printf("我是子进程。我的PID是%d,我爹的PID是%d。fork 返回给我的值是：%d\n",getpid(),getppid(),pid);
    }else {
        //这里的代码只会被父进程执行
        printf("我是父进程。我的PID是:%d。 fork 返回给我的值:%d(也就是子进程的PID)\n",getpid(),pid);
    }
    return 0;
}


