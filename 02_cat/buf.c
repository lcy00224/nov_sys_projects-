#include <stdio.h>
#include <unistd.h>
int main() {
    printf("我是printf (带缓冲区）");
    write(1,"我是buf (直接进内核）\n",31);
    fflush(stdout);
    while(1) {
        sleep(1);
    }
    return 0;
}
