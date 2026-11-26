#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
int main (int argc, char *argv[]) {
    if (argc == 1) {
        printf("请输入<源文件> <文件>\n");
        return 0;
    } else if (argc == 2) {
        int fd = open(argv[1], O_RDONLY);
        if (fd == -1) {
            perror("文件打开失败\n");
            exit(1);
        }
        char buf[1024];
        ssize_t n;
        while ((n = read(fd, buf, sizeof(buf))) > 0) {
            write(1, buf, n);
        }
    } else {
        perror("输入有误");
        exit(1);
    }
    return 0;
}
