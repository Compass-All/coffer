#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define NUM_THREADS 32
#define MESSAGE "Hello, Server!"

void *client_thread(void *arg) {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[1024];
    int thread_id = *(int *)arg;

    // 创建套接字
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        pthread_exit(NULL);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    // 将 IPv4 地址从文本转换为二进制格式
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid address or Address not supported");
        close(sockfd);
        pthread_exit(NULL);
    }

    // 连接到服务器
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sockfd);
        pthread_exit(NULL);
    }

    // 发送消息到服务器
    // send(sockfd, MESSAGE, strlen(MESSAGE), 0);
    write(sockfd, MESSAGE, strlen(MESSAGE));
    printf("Thread %d: Message sent\n", thread_id);

    // 从服务器接收响应
    int bytes_received = read(sockfd, buffer, sizeof(buffer) - 1);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';  // 确保字符串以 null 终止
        printf("Thread %d: Received from server: %s\n", thread_id, buffer);
    } else {
        printf("Thread %d: No response received\n", thread_id);
    }

    // 关闭套接字
    close(sockfd);
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; ++i) {
        thread_ids[i] = i;
        if (pthread_create(&threads[i], NULL, client_thread, &thread_ids[i]) != 0) {
            perror("Failed to create thread");
            return -1;
        }
    }

    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
