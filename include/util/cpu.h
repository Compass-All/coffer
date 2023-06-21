#pragma once

#define FENCE() __asm__ __volatile__ ("fence " "rw" "," "rw" : : : "memory")


#ifndef __ASSEMBLY__
#ifndef likely
#define likely(x)   (__builtin_expect((!!(x)), 1))
#endif

#ifndef unlikely
#define unlikely(x) (__builtin_expect((!!(x)), 0))
#endif

#ifndef barrier
// #define barrier() __asm__ __volatile__("" : : : "memory")
#define barrier() FENCE()
#endif

#ifndef mb
#define mb()      barrier()
#endif

#ifndef rmb
#define rmb()     barrier()
#endif

#ifndef wmb
#define wmb()     barrier()
#endif
#endif /* !__ASSEMBLY__ */