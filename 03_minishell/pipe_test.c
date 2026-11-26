#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main() {
    int fd[2];
    pipe(fd);
    pid_t pid = fork();
    if (pid < 0) {
        printf("fork失败\n");
    } else if (pid == 0) {
        close(fd[0]);
        const char *buf = "hello,world";
        write(fd[1], buf, strlen(buf) + 1);
        close(fd[1]);
    } else {
        close(fd[1]);
        char buf[100];
        ssize_t n;
        n = read(fd[0], buf, sizeof(buf));
        buf[n] = '\0';
        printf("读取到：%s\n", buf);
        close(fd[0]);
        waitpid(pid, NULL, 0);
    }
    return 0;
}
