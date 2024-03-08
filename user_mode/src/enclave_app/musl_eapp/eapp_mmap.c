#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#define NUM_SEGMENTS 512
#define SEGMENT_SIZE (2 * 1024 * 1024) // 2MB
#define ITER_TIMES 60

int main() {
    // 保存内存段指针的数组
    char *segments[NUM_SEGMENTS];

    // 使用mmap分配内存
    for (int i = 0; i < NUM_SEGMENTS; ++i) {
        segments[i] = mmap(NULL, SEGMENT_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (segments[i] == MAP_FAILED) {
            perror("mmap");
            exit(EXIT_FAILURE);
        }
    }

    // 访问内存片段
    for (int cnt=0; cnt < ITER_TIMES; ++cnt) {
        for (int i = 0; i < NUM_SEGMENTS; ++i) {
            // 例如，可以写入内存片段
            for (size_t j = 0; j < SEGMENT_SIZE; ++j) {
                segments[i][j] = (char)(cnt); // 使用简单的数据填充内存
            }
        }
    }

    // 访问内存片段
    for (int i = 0; i < NUM_SEGMENTS; ++i) {
        // 读取内存片段
        for (size_t j = 0; j < SEGMENT_SIZE; ++j) {
            volatile char data = segments[i][j]; // 读取内存数据，声明为volatile以避免编译器优化
        }
    }

    // 释放内存
    for (int i = 0; i < NUM_SEGMENTS; ++i) {
        if (munmap(segments[i], SEGMENT_SIZE) == -1) {
            perror("munmap");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
