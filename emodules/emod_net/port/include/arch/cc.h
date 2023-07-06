#pragma once

#include <types.h>
#include "../../../dependency.h"

#define LWIP_ERR_T int

/* Define byte ordering */
// todo: check risc-v asm for htos, htol functions
#ifndef BYTE_ORDER
#define BYTE_ORDER LITTLE_ENDIAN
#endif

/* Platform byte ops */
#define LWIP_PLATFORM_BYTESWAP 1
#define LWIP_PLATFORM_HTONS(x) 	((((x) & 0xff) << 8) | (((x) & 0xff00) >> 8))
#define LWIP_PLATFORM_HTONL(x)	( \
        (((x) & 0xFF) << 24) | \
        ((((x) >> 8) & 0xFF) << 16) | \
        ((((x) >> 16) & 0xFF) << 8) | \
        (((x) >> 24) & 0xFF) \
    )
#define lwip_htons(x) LWIP_PLATFORM_HTONS(x)
#define lwip_htonl(x) LWIP_PLATFORM_HTONL(x)
#define lwip_itoa(x, y, z) snprintf(x, y, "%d", z)

/* Package structure */
#define PACK_STRUCT_FIELD(x) x __attribute__((packed))
#define PACK_STRUCT_STRUCT __attribute__((packed))
#define PACK_STRUCT_BEGIN
#define PACK_STRUCT_END

/* Platform specific diagnostic output */
#define LWIP_PLATFORM_DIAG(x) do { printf x; } while(0)
#define LWIP_PLATFORM_ASSERT(x) do { printf("Assertion \"%s\" failed at line %d in %s\n", \
    x, __LINE__, __FILE__); } while(0)

/* Platform specific locking */
/* todo: 
 *      - consider change this to atomic variable;
 *      - then move the following macro definition to sys_arch.h/sys_arch.c;
 *          (ref: https://lwip.fandom.com/wiki/Porting_for_an_OS)
 */
#define SYS_ARCH_PROTECT(x) do { } while(0)
#define SYS_ARCH_UNPROTECT(x) do { } while(0)
#define SYS_ARCH_DECL_PROTECT(x) u32_t x

#ifndef __offsetof
#define __offsetof(t, field) \
	((__sz)(__uptr)((const volatile void *)&((t *)0)->field))
#endif

#ifndef DEQUALIFY
#define DEQUALIFY(t, a) ((t)(__uptr)(const volatile void *)(a))
#endif

#ifndef __containerof
#define __containerof(x, s, m) \
	DEQUALIFY(s *, (const volatile char *)(x) - __offsetof(s, m))
#endif

#ifndef DECONST
#define DECONST(t, a) ((t)((__uptr)((const void *)(a))))
#endif

#ifndef UK_CTASSERT
#define UK_CTASSERT(x)             _UK_CTASSERT(x, __LINE__)
#define _UK_CTASSERT(x, y)         __UK_CTASSERT(x, y)
#define __UK_CTASSERT(x, y)        typedef __maybe_unused \
	char __assert_ ## y [(x) ? 1 : -1]
#endif /* UK_CTASSERT */