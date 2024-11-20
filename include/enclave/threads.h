#pragma  once

#define NUM_ENCLAVE     32

// #if defined __QEMU__
// #define NUM_ENCLAVE     32
// #elif defined __UNMATCHED__
// #define NUM_ENCLAVE     16
// #elif defined __VISIONFIVE2__
// #define NUM_ENCLAVE     16
// #else
// #error "Unknown Platform"
// #endif 

#define THREADS_LIMIT   32
#define __NUM_THREADS   (1024 / NUM_ENCLAVE)
#define NUM_THREADS     (__NUM_THREADS > THREADS_LIMIT ? THREADS_LIMIT : __NUM_THREADS)
// #define NUM_THREADS    16
#define MAX_FORK     4
