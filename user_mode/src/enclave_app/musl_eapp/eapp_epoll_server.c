#include <sys/epoll.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define MAX_EVENTS 128
#define PORT 8080
#define BUFFER_SIZE 1024

int set_nonblocking(int fd) {
    int opts = fcntl(fd, F_GETFL);
    if (opts < 0) {
        perror("fcntl(F_GETFL)");
        return -1;
    }
    opts = (opts | O_NONBLOCK);
    if (fcntl(fd, F_SETFL, opts) < 0) {
        perror("fcntl(F_SETFL)");
        return -1;
    }
    return 0;
}

int main() {
    int server_fd, new_socket, epoll_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    struct epoll_event event, events[MAX_EVENTS];
    int opt = 1;

    // 创建监听套接字
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 设置套接字选项（例如，允许端口重用）
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) != 0) {
        perror("setsockopt failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 设置服务器地址和端口
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // 绑定地址
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 监听端口
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 创建epoll实例
    if ((epoll_fd = epoll_create1(0)) == -1) {
        perror("epoll_create1");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 将服务器套接字添加到epoll监听列表
    event.events = EPOLLIN;
    event.data.fd = server_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event) == -1) {
        perror("epoll_ctl: server_fd");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("epoll_ctl: epoll_fd = %d\n", epoll_fd);
    printf("epoll_ctl: server_fd = %d\n", server_fd);

    printf("Listening on port %d...\n", PORT);

    while (1) {
        int n_ready, i;
        n_ready = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (n_ready == -1) {
            perror("epoll_wait");
            break;
        }

        for (i = 0; i < n_ready; i++) {
            if (events[i].data.fd == server_fd) {
                // 接受新的连接
                new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
                if (new_socket < 0) {
                    perror("accept");
                    continue;
                }

                // 设置为非阻塞模式
                if (set_nonblocking(new_socket) < 0) {
                    close(new_socket);
                    continue;
                }

                // 使用 getpeername 获取客户端的 IP 和端口
                struct sockaddr_in peer_addr;
                socklen_t peer_addr_len = sizeof(peer_addr);
                if (getpeername(new_socket, (struct sockaddr *)&peer_addr, &peer_addr_len) == -1) {
                    perror("getpeername failed");
                } else {
                    printf("New connection from %s:%d\n",
                           inet_ntoa(peer_addr.sin_addr),
                           ntohs(peer_addr.sin_port));
                }

                // 将新连接添加到epoll监听列表
                printf("Adding new_socket to epoll: fd = %d\n", new_socket);
                event.events = EPOLLIN | EPOLLET;
                event.data.fd = new_socket;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_socket, &event) == -1) {
                    perror("epoll_ctl: new_socket");
                    close(new_socket);
                }

            } else {
                printf("events[%d].data.fd = %d\n", i, events[i].data.fd);

                // 处理已经连接的客户端发送的数据
                char buffer[BUFFER_SIZE];
                // int bytes_read = read(events[i].data.fd, buffer, sizeof(buffer));  // Bug: panic at read fd = 0 !!
                int bytes_read = recv(events[i].data.fd, buffer, sizeof(buffer), 0);
                if (bytes_read > 0) {  // 读取成功
                    // write(events[i].data.fd, buffer, bytes_read);
                    send(events[i].data.fd, buffer, bytes_read, 0);
                } else if (bytes_read == 0) {  // 客户端关闭
                    printf("Client disconnected\n");
                    close(events[i].data.fd);
                } else {  // 读取错误
                    perror("read");
                    close(events[i].data.fd);
                }
            }
        }
    }

    close(server_fd);
    return 0;
}
