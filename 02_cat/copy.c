#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc ,char *argv[]) {
    if (argc != 3) {
        printf("用法: %s <源文件> <目标文件>\n",argv[0]);
        exit(1);
    }
    int fd_in = open(argv[1], O_RDONLY);
    if (fd_in == -1) {
        perror("打开源文件失败");
        exit(1);
    }
    int fd_out = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_out == -1) {
        perror("打开目标文件失败");
        exit(1);
    }
    char buf[1024];
    ssize_t n;
    while ((n =read(fd_in, buf, sizeof(buf))) > 0) {
        write(fd_out, buf, n);
    }
    close(fd_in);
    close(fd_out);
    return 0;
}  
