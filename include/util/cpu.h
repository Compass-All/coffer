#pragma once

#include <memory/riscv_barrier.h>

#define FENCE() RISCV_FENCE(rw, rw)


#ifndef __ASSEMBLY__
#ifndef likely
#define likely(x)   (__builtin_expect((!!(x)), 1))
#endif

#ifndef unlikely
#define unlikely(x) (__builtin_expect((!!(x)), 0))
#endif

#ifndef barrier
#define barrier() FENCE()
#endif


#endif /* !__ASSEMBLY__ */