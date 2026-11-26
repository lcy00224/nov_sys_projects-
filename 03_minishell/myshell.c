#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_LINE 1024
#define MAX_ARGS 64

int main() {
    char line[MAX_LINE];
    char *argv[MAX_ARGS];
    while(1) {
        while (waitpid(-1, NULL, WNOHANG) > 0);
        printf("myshell$ ");
        fflush(stdout);
        //读取用户输入
        if (fgets(line, MAX_LINE, stdin) == NULL) {
            //fgets(char *s, int size, FILE *stream) 从指定的输入流中读取最多size-1个字符的一行,存入s,返回s的地址
            printf("\n退出 shell\n");
            break;
        }
        
        //解析输入（把换行符去掉，把字符串切成 argv 数组）
        line[strcspn(line, "\n")] = 0;//扫描字符串line, 找到第一个\n出现的位置，返回下标
        if (strlen(line) == 0) {
            continue;//如果只敲了回车，直接跳过，打印提示符
        }
        //使用strtok拆分字符串
        int argc = 0;
        char *token = strtok(line," ");//以第一个空格为界拆分，返回被切下第一个片段的指针
        while (token != NULL && argc < MAX_ARGS-1) {
            argv[argc++] = token;
            token = strtok(NULL, " ");//传NULL,意思是从上次切的位置继续切。
        }
        argv[argc] = NULL;//极其重要，参数数组必须以NULL结尾！
        //检查是否有管道
        int pipe_index = -1;
        for ( int i=0; i<argc; i++){
            if (strcmp(argv[i], "|") == 0) {
                pipe_index = i;
                break;
            }
        }
        if (pipe_index != -1) {
            argv[pipe_index] = NULL;
            char **argv2 = &argv[pipe_index+1];
            int fd[2];
            pipe(fd);
            pid_t pid1 = fork();
            if (pid1 == 0) {
                close(fd[0]);
                dup2(fd[1],1);
                close(fd[1]);
                execvp(argv[0], argv);
                perror("fork1 false");
                exit(1);
            }
            pid_t pid2 = fork();
            if (pid2 == 0) {
                close(fd[1]);
                dup2(fd[0], 0);
                close(fd[0]);
                execvp(argv2[0], argv2);
                perror("fork2 false");
                exit(1);
            }
            close(fd[0]);
            close(fd[1]);
            waitpid(pid1, NULL, 0);
            waitpid(pid2, NULL, 0);
            continue;
        }
        int is_background = 0;
        if (argc > 0 && strcmp(argv[argc-1], "&") == 0) {
            is_background = 1;
            argv[argc-1] = NULL;
            argc--;
        }
        //处理exit 命令
        if (strcmp(argv[0], "exit") == 0) {
            printf("Bye!\n");
            break;
        }
        // 【核心补丁 1】拦截处理 cd（内建命令）
        if (strcmp(argv[0], "cd") == 0) {
            if (argc < 2) {
                chdir(getenv("HOME")); // 回主目录
            } else {
                if (chdir(argv[1]) != 0) {
                    perror("cd 失败"); // 切换失败
                }
            }
            continue; // 处理完必须跳过 fork
        }
        //进程控制
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork 失败");
            exit(1);
        } else if (pid == 0) {
            execvp(argv[0], argv);
            // 如果返回，说明命令不存在
            perror("命令执行失败");
        }else {
             if (!is_background) {
                 waitpid(pid, NULL, 0);
             }
        }    
        
    }
    return 0;
}
