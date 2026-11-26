#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <fcntl.h>
#define MAX_EVENTS 10
void set_notblocking(int fd) {
    int flags = fcntl(fd, F_SETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
} 

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8080);
    bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
    listen(server_fd, 5);
    
    int epfd = epoll_create(1);
    struct epoll_event ev, events[MAX_EVENTS];
    ev.events = EPOLLIN;  
    ev.data.fd = server_fd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, server_fd, &ev);
    printf("epoll服务器启动，正在监听8080...\n");
    while (1) {
        int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
        for (int i=0; i<nfds; i++) {
            if (events[i].data.fd == server_fd) {
                int client_fd = accept(server_fd, NULL, NULL);
                // 把新客户也注册到监控中心
                ev.events = EPOLLIN;
                ev.data.fd = client_fd;
                epoll_ctl(epfd, EPOLL_CTL_ADD, client_fd, &ev);
                printf("新客户端连入: fd = %d\n", client_fd);
            } else {
                int client_fd = events[i].data.fd;
                char buf[1024];
                int n = read(client_fd, buf, sizeof(buf));
                if (n>0) {
                    write(client_fd, buf, n);
                } else if(n ==0) {
                    printf("客户端断开\n");
                    close(client_fd);
                    epoll_ctl(epfd, EPOLL_CTL_DEL, client_fd, NULL);
                }
            }
        }
    }
    return 0;
}
