#pragma  once

#if defined __QEMU__
#define NUM_ENCLAVE     64
#elif defined __UNMATCHED__
#define NUM_ENCLAVE		32
#else
#error "Unknown Platform"
#endif 

#define THREADS_LIMIT   8
#define __NUM_THREADS   (1024 / NUM_ENCLAVE)
#define NUM_THREADS     (__NUM_THREADS > THREADS_LIMIT ? THREADS_LIMIT : __NUM_THREADS)
#define MAX_FORK     4

#define FUTEX_WAIT		0
#define FUTEX_WAKE		1
