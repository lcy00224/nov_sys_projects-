#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    // 1. 创建客户端的 Socket
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1) {
        perror("socket failed");
        exit(1);
    }

    // 2. 填写你要连接的服务器地址（本机 127.0.0.1，端口 8080）
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    
    // 将字符串 IP 转换为网络能懂的整数 IP
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    // 3. 主动发起连接（拨号）
    if (connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect failed");
        exit(1);
    }
    printf("已成功连接到服务器 127.0.0.1:8080\n");
    for (int i=0; i<3; i++) {
        // 4. 核心：按照我们自定义的协议打包数据
        const char *msg = "Hello Linux! I am client!";
        uint32_t msg_len = strlen(msg); // 获取实际数据长度

        // 按照协议：先发 4 字节的包头（网络字节序）
        uint32_t net_len = htonl(msg_len); // 将主机字节序转成网络字节序
        write(sock_fd, &net_len, sizeof(net_len)); // 发送包头
    
        // 接着发真正的数据包体
        write(sock_fd, msg, msg_len); 
        printf("已发送数据: %s (长度: %u 字节)\n", msg, msg_len);
    }
    // 5. 接收服务器的回显（验证服务器是否处理成功）
    char buf[1024] = {0};
    int n = read(sock_fd, buf, sizeof(buf) - 1);
    if (n > 0) {
        printf("收到服务器回显: %s\n", buf);
    } else {
        printf("未收到服务器回显或连接已断开\n");
    }

    // 6. 关闭连接
    close(sock_fd);
    return 0;
}
