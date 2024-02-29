#include <sbi/ebi/tklock.h>
#include <sbi/ebi/ebi_debug.h>

void tklock_fetch(tklock_t* lock)
{
    long t = atomic_add_return(&lock->next, 1);
    long owner = atomic_read(&lock->owner);
    sbi_debug("ticket #%lu, owner=%lu\n", t, owner);
    while (t != owner)
        owner = atomic_read(&lock->owner);
}

void tklock_release(tklock_t* lock)
{
    atomic_add_return(&lock->owner, 1);
}