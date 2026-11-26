#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 1024
#define MAX_ARGS 5
int main() {
    char line[MAX_SIZE];
    char *argv[MAX_ARGS];
    while (1) {
        printf("myshell$" );
        if (fgets(line, MAX_SIZE, stdin) == 0) {
            printf ("输入失败\n");
            continue;
        }
        line[strcspn(line, "\n")] = 0;
        if (strlen(line) == 0) {
            continue;
        }
        int argc = 0;
        char *token = strtok(line, " ");
        while (token != NULL) {
            argv[argc] = token;
            argc++;
            token = strtok(NULL, " ");
        }
        argv[argc] = 0;
        if (strcmp(argv[0], "exit") == 0) {
            break;
        }
        if (strcmp(argv[0], "cd") == 0) {
            if (argc <2) {
                chdir(getenv("home"));
            }else {
                if (chdir(argv[1]) != 0) {
                    perror("cd失败\n");
                }
            }
            continue;
        }
        pid_t pid = fork();
        if (pid < 0) {
            printf("fork 失败\n");
            exit(1);
        } else if (pid == 0) {
            execvp(argv[0], argv);
            perror("子进程替换失败");
            exit(1);
        } else {
            waitpid(pid, NULL,0);
        }
    }
    return 0;
}
