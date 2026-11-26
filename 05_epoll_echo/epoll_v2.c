#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>
#define MAX_EVENTS  5
#define MAX_MSG 1024
void no_blocking(int fd){
    int flags = fcntl (fd, F_GETFL);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

typedef struct {
    int fd;
    char buf[1024];
    int len;
} clientdata;

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(server_fd, (const struct sockaddr*)&addr, sizeof(addr));
    listen(server_fd, 5);
    printf("服务端已启动...\n");
    int epfd = epoll_create(1);
    struct epoll_event events[MAX_EVENTS], ev;
    ev.events = EPOLLIN;
    ev.data.fd = server_fd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, server_fd, &ev);
    clientdata buffer[1024];
    void handle_read (int client_fd, int epfd) {
        clientdata *client = &buffer[client_fd];
        int n = read(client_fd, client->buf + client->len, MAX_MSG-client->len);
        if (n>0) {
            client->len += n;
            while (client->len > 4) {
                int msg_len;
                memcpy(&msg_len, client->buf, 4);
                msg_len = ntohl(msg_len);
                printf("收到包头: %d\n", msg_len);
                if ( n >= msg_len +4) {
                    char *msg = client->buf + 4;
                    printf("收到完整数据%.*s\n", msg_len, msg);
                    write(client_fd, msg, msg_len);
                    int pakage_len = msg_len + 4;
                    int remain_len = client->len - pakage_len;
                    memmove(client->buf, client->buf + pakage_len, remain_len);
                    client->len = remain_len;
                } else {
                    break;
                }
            }
        } else if (n == 0) {
            epoll_ctl(epfd, EPOLL_CTL_DEL, client_fd, NULL);
            close(client_fd);
            printf("客户端断开, fd:%d\n", client_fd);
        } else if (n == -1) {
            // 🚨 核心考点：怎么区分是真的报错了，还是只是没数据了？
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    // 暂时没数据可读，这是非阻塞正常现象，无视它
            } else {
                    // 真正的读错误（比如连接被重置），需要断开
                perror("read error");
                epoll_ctl(epfd, EPOLL_CTL_DEL, client_fd, NULL);
                close(client_fd);
            }
        }
    }
    while (1) {
        int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
        for (int i =0; i<nfds; i++) {
            if (events[i].data.fd == server_fd) {
                int client_fd = accept(server_fd, NULL, NULL);
                no_blocking(client_fd);
                ev.events = EPOLLIN;
                ev.data.fd = client_fd;
                epoll_ctl(epfd, EPOLL_CTL_ADD, client_fd, &ev);
                printf("连接到新的客户端，fd:%d\n", client_fd);
            } else {
                int client_fd = events[i].data.fd;
                handle_read(client_fd, epfd);    
            }
        }
    }

    return 0;
}
