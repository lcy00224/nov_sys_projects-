#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

#define MAX_LINE 1024
#define MAX_ARGS 5
int main () {
    while (1) {
        while (waitpid(-1, NULL, WNOHANG) >0){};
        printf("myshell$ ");
        fflush(stdout);
        char line[MAX_LINE];
        if (fgets(line, MAX_LINE, stdin) == NULL) {
            perror("输入错误");
            exit(1);
        }
        ssize_t n;
        if ((n = strcspn(line, "\n")) == 0) {
            continue;
        }
        line[n] = 0;
        int argc = 0;
        char *argv[MAX_ARGS];
        memset(argv, 0, sizeof(argv));
        char *token = strtok(line," ");
        while (token != NULL) {
            argv[argc++] = token;
            token = strtok(NULL, " ");
        }
        argv[argc] = NULL;
        if (strcmp(argv[0], "exit") == 0) {
            break;
        }
        if (strcmp(argv[0], "cd") == 0) {
            if (argc < 2) {
                chdir(getenv("HOME"));
            }else{
                chdir(argv[1]);
            }
            continue;
        }
        int pipe_index = -1;
        int nowait = 0;
        for (int i = 0;i<argc;i++){
            if (strcmp(argv[i], "|") == 0){
                pipe_index = i;
            }
            if (strcmp(argv[i], "&") == 0) {
                argv[i] = NULL;
                nowait = 1;
            }
        }
        if (pipe_index > 0) {
            argv[pipe_index] = NULL;
            char **argv2 =&argv[pipe_index+1];
            int fd[2];
            pipe(fd);
            if (fork() == 0) {
                close(fd[0]);
                dup2(fd[1], 1);
                close(fd[1]);
                execvp(argv[0], argv);
                perror("fork1 false");
                exit(1);
            }
            if (fork() == 0){
                close(fd[1]);
                dup2(fd[0], 0);
                close(fd[0]);
                execvp(argv2[0], argv2);
                perror("fork2 false");
                exit(1);
            }
            close(fd[0]);
            close(fd[1]);
            if (nowait == 0 ) {
                waitpid(-1, NULL, 0);
                waitpid(-1, NULL, 0);
            }
            continue;
        }
        pid_t pid = fork();
        if (pid == 0){
            execvp(argv[0], argv);
            perror("exec false");
            exit(1);
        } else if (pid < 0) {
            perror("fork false");
            exit(1);
        } else {
            if (nowait == 0) {
                waitpid(pid, NULL, 0);
            }
        }
    } 
    return 0;
}
