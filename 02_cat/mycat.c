#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int fd;
    if (argc == 1) {
        fd = 0;
    } else {
        for (int i=1;i<argc;i++) {
            fd = open(argv[i], O_RDONLY);
            if (fd == -1) {
                perror("argv[i]");
                continue;
            }
            char buf[1024];
            ssize_t n;
            while ((n = read (fd, buf, sizeof(buf))) > 0) {
                write(1, buf, n);
            }
            if (argc > 1) {
                close(fd);
            }
        }
    }
    return 0;
}
