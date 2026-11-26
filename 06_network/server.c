#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0){
        perror("socket failed");
        exit(1);
    }
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8080);
    
    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1){
        perror("bind failed");
        exit(1);
    }
    
    if (listen(server_fd, 5) == -1){
        perror("listen failed");
        exit(1);
    }
    
    printf("服务器启动成功，正在监听端口8080...\n");
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
    if (client_fd == -1) {
        perror("accept failed");
        exit(1);
    }
    printf("有客户端连进来了...\n");
    
    char buf[1024];
    ssize_t n;
    while ((n = read(client_fd, buf, sizeof(buf))) > 0) {
        write(client_fd, buf, n);
    }
    
    close(client_fd);
    close(server_fd);
    return 0;
}
