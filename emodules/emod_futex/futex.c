#include "futex.h"
#include "emodules/debug.h"
#include "emodules/dependency.h"
#include "emodules/grand_lock.h"
#include <emodules/emod_futex/emod_futex.h>
#include "lock.h"
#include "message/short_message.h"
#include "types.h"
#include "atomic.h"
#include <util/list.h>
#include <memory/memory.h>
#include <enclave/enclave_ops.h>
#include <enclave/threads.h>
#include <util/errno.h>
#include <util/gnu_attribute.h>
#include "dependency.h"

#define list_head           uk_list_head
#define list_for_each       uk_list_for_each
#define list_entry          uk_list_entry
#define list_add_tail       uk_list_add_tail
#define list_del            uk_list_del
#define INIT_LIST_HEAD      UK_INIT_LIST_HEAD
#define ARRAY_SIZE(x) 	(sizeof(x) / sizeof((x)[0]))

/* These mutexes are a very simple counter: the winner is the one who
     decrements from 1 to 0.    The counter starts at 1 when the lock is
     free.    A value other than 0 or 1 means someone may be sleeping.
     This is simple enough to work on all architectures, but has the
     problem that if we never "up" the semaphore it could eventually
     wrap around. */

/* FIXME: This may be way too small. --RR */
#define FUTEX_HASHBITS 6

/* We use this instead of a normal wait_queue_t, so we can wake only
     the relevent ones (hashed queues may be shared) */
struct futex_q {
    struct list_head list;
    u64 tid;
    /* Page struct and offset within it. */
    vaddr_t uaddr;
    // struct page *page;
    // unsigned int offset;
};

/* The key for the hash is the address + index + offset within page */
static struct list_head futex_queues[1 << FUTEX_HASHBITS];
static spinlock_t futex_lock = SPIN_LOCK_INITIALIZER;

static inline unsigned long hash_long(unsigned long val, unsigned int bits) {
    unsigned long hash = val * 0x61C8864680B583EBull;
    return hash >> (64 - bits);
}

__unused static inline struct list_head *hash_futex(vaddr_t uaddr) {
    /* struct page is shared, so we can hash on its address */
    return &futex_queues[hash_long(uaddr, FUTEX_HASHBITS)];
}

static inline void wake_one_waiter(struct list_head *head, vaddr_t uaddr) {
    struct list_head *i;

    show(head);
    spin_lock(&futex_lock);
    list_for_each(i, head) {
        struct futex_q *this = list_entry(i, struct futex_q, list);
        show(i);
        show(i->next);
        show(this);

        if (this->uaddr == uaddr) {
            __ecall_ebi_unblock_threads(1UL << this->tid);
            break;
        }
    }
    spin_unlock(&futex_lock);
}

/* Add at end to avoid starvation */
static inline void queue_me(struct list_head *head, struct futex_q *q,
    vaddr_t uaddr)
{
    show(q);
    show(head);

    info("CP1\n");

    q->tid = __ecall_ebi_get_tid();
    q->uaddr = uaddr;

    spin_lock(&futex_lock);
    list_add_tail(&q->list, head);
    spin_unlock(&futex_lock);

    struct list_head *i;
    list_for_each(i, head) {
        show(i);
        show(i->next);
    }

    info("CP2\n");
}

static inline void unqueue_me(struct futex_q *q) {
    spin_lock(&futex_lock);
    list_del(&q->list);
    spin_unlock(&futex_lock);

    struct list_head *i;
    list_for_each(i, q->list.prev) {
        show(i);
        show(i->next);
    }
}

/* Try to decrement the user count to zero. */
static int decrement_to_zero(vaddr_t uaddr) {
    atomic_t *count;
    count = (void *)uaddr;
    int ret = 0;

    int cur_val = atomic_read(count);
    if (cur_val >= 0 && atomic_sub_return(count, 1) == 0)
        ret = 1;

    info("Futex at 0x%lx current value: 0x%lx\n", uaddr, cur_val);
    cur_val = atomic_read(count);
    info("Futex at 0x%lx changed to 0x%lx\n", uaddr, cur_val);

    return ret;
}

/* Simplified from arch/ppc/kernel/semaphore.c: Paul M. is a genius. */
__unused int futex_down(struct list_head *head, vaddr_t uaddr, int val) {
    int retval = 0;
    struct futex_q q;
    u64 tid = __ecall_ebi_get_tid();
    int first = 1;
    info("thread %lu getting down\n", tid);

    show(head);
    show(uaddr);
    show(val);

    queue_me(head, &q, uaddr);

    info("CP1\n");

    while (!decrement_to_zero(uaddr)) {
        if (first) {
            // __ecall_ebi_block_thread(tid);
            first = 0;
        }
        spin_unlock_grand();
        __ecall_ebi_suspend(BLOCKED);
        spin_lock_grand_suspend();
    }

    info("CP2\n");

    unqueue_me(&q);
    info("CP3\n");

    /* If we were signalled, we might have just been woken: we
         must wake another one.    Otherwise we need to wake someone
         else (if they are waiting) so they drop the count below 0,
         and when we "up" in userspace, we know there is a
         waiter. */
    wake_one_waiter(head, uaddr);
    info("CP4\n");
    return retval;
}

__unused int futex_up(struct list_head *head, vaddr_t uaddr, int val) {
    atomic_t *count;

    count = (void *)uaddr;
    atomic_write(count, val);
    smp_wmb();
    wake_one_waiter(head, uaddr);
    return 0;
}

static int futex_wait(vaddr_t uaddr, u32 val)
{
    atomic_t *atom_val = (void *)uaddr;

    int cur_val = atomic_read(atom_val);
    // DEBUG("uaddr = %x, val = %u, cur_val = %d\n", uaddr, val, cur_val);

    if (cur_val != val) {
        return -EWOULDBLOCK;
    }

    u64 tid = __ecall_ebi_get_tid();
    __ecall_ebi_block_thread(tid);
    spin_unlock_grand();
    // DEBUG("Blocking thread %lu now!\n", tid);
    __ecall_ebi_suspend(BLOCKED);
    spin_lock_grand_suspend(); 

    return 0;
}

static int futex_wake(vaddr_t uaddr, int nr_wake)
{
    
    atomic_t *atom_val = (void *)uaddr;
    __unused int cur_val = atomic_read(atom_val);
    u64 threads_to_unblock = 0UL;
    u64 blocked_threads = __ecall_ebi_get_blocked_threads();
    
    // DEBUG("uaddr = %0lx, cur_val = %d, nr_wake = %d\n", (u64)uaddr, cur_val, nr_wake);
    // LOG(blocked_threads);
    if (blocked_threads == 0) {
        // show("No blocked threads to wake, early return 0\n");
        return 0;
    }

    int cnt = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        if (cnt == nr_wake)
            break;

        if (blocked_threads & (1UL << i)) {
            threads_to_unblock |= 1UL << i;
            cnt++;
        }
    }

    if (cnt < nr_wake) {
        show(cnt); show(nr_wake);
        // DEBUG("No enough blocked threads to wake\n");
    } 
    if (threads_to_unblock != 0) {
        // DEBUG("Waking threads(bitmap) 0x%lx\n", threads_to_unblock);
        __ecall_ebi_unblock_threads(threads_to_unblock);
    }
    
    return 0;
}

// shall we duplicate kernel stack?

static int sys_futex(vaddr_t uaddr, int op, int val) {
    int ret;
    unsigned long pos_in_page;
    pos_in_page = ((unsigned long)uaddr) % PAGE_SIZE;
    
    // show(pos_in_page);
    // show((pos_in_page % __alignof__(atomic_t)));
    // show((pos_in_page + sizeof(atomic_t)));
    // show((u64)uaddr);
    // show(op);
    // show(val);
    
    /* Must be "naturally" aligned, and not on page boundary. */
    /*
     * On all platforms, futexes are four-byte integers 
     * that must be aligned on a four-byte boundary.  
     */
    if ((pos_in_page % __alignof__(atomic_t)) != 0 ||
            pos_in_page + sizeof(atomic_t) > PAGE_SIZE)
        return -EINVAL;
    int cmd = op & FUTEX_CMD_MASK;
    switch (cmd) {
    case FUTEX_WAKE:
        ret = futex_wake(uaddr, val);
        break;
    case FUTEX_WAIT:
        ret = futex_wait(uaddr, val);
        break;
    /* Add other lock types here... */
    default:
        show(uaddr); show(op); show(val);
        panic("unhandled cases in sys_futex()...\n");
        ret = -EINVAL;
    }

    return ret;
}

// long syscall(SYS_futex, uint32_t *uaddr, int futex_op, uint32_t val,
// 		const struct timespec *timeout,   /* or: uint32_t _val2_ */
//         uint32_t *uaddr2, uint32_t val3)
int sys_futex_handler(u32 *uaddr, int futex_op, int val, u64 _2, u64 _3, u64 _4)
{
    return sys_futex((vaddr_t)uaddr, futex_op, val);
}

int futex_init(void) {
    unsigned int i;

    for (i = 0; i < ARRAY_SIZE(futex_queues); i++)
        INIT_LIST_HEAD(&futex_queues[i]);
    return 0;
}