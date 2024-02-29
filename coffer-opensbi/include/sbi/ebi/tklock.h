#ifndef EBI_TKLOCK_H
#define EBI_TKLOCK_H

#include <sbi/riscv_atomic.h>

typedef struct {
    atomic_t owner;
    atomic_t next;
} tklock_t;

#define tklock_init(lock)               \
    do {                               \
        ATOMIC_INIT(&(lock)->owner, 0); \
        ATOMIC_INIT(&(lock)->next, -1);  \
    } while (0)

#define TKLOCK_INITIALIZER {         \
    .owner = ATOMIC_INITIALIZER(0), \
    .next = ATOMIC_INITIALIZER(-1),  \
};

void tklock_fetch(tklock_t* lock);
void tklock_release(tklock_t* lock);

#endif