#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
    printf("--- 父进程开始，PID:%d\n",getpid());
    pid_t pid = fork();
    if (pid == 0) {
        printf("子进程开始执行ls\n");
        char *argv[]={"ls","-l","/",NULL};
        execvp("ls",argv);
        perror("替换失败");
        exit(1);
    }else if (pid > 0) {
        printf("子进程PID: %d,现在它开始等死\n",pid);
        int status;
        waitpid(pid, &status, 0);
        // waitpid 会阻塞在这里，直到子进程 pid 运行结束
        if (WIFEXITED(status)) {
            printf("父进程：子进程结束，退出码是 %d\n", WEXITSTATUS(status));
        }
    }
    return 0;
}
