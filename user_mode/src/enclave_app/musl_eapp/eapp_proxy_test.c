// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

// #include <util/register.h>
#define read_csr(reg)                 \
    ({                                \
        unsigned long __tmp;          \
        __asm__ __volatile__("csrr %0, " #reg \
                    : "=r"(__tmp));   \
        __tmp;                        \
    })


#define PORT 8080
#define BUFFER_SIZE 1024

// 读取time CSR的函数
// static inline uint64_t read_time() {
//     uint64_t time;
//     // 使用RISC-V的内联汇编语法来读取time CSR
//     asm volatile ("rdtime %0" : "=r"(time));
//     return time;
// }

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};


    printf("Creating socket...\n");
    // 创建套接字
    clock_t start = read_csr(cycle);
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    clock_t end = read_csr(cycle);

    if ( server_fd== 0) {
        perror("socket failed");
        // exit(EXIT_FAILURE);
    }

    printf("eapp socket ret = %d, duration: %lu cycles\n", server_fd, (end - start));
    printf("eapp start = %lu\neapp end = %lu\n", start, end);
    return 0;
}
