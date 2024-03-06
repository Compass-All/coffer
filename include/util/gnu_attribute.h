#pragma once

#ifndef __unused
#define __unused		__attribute__((unused))
#endif
#ifndef __diverge
#define __diverge		__attribute__((noreturn))
#endif
#ifndef __page_aligned
#define __page_aligned	__attribute__((aligned(PAGE_SIZE)))
#endif
#ifndef __packed
#define __packed        __attribute__((packed))
#endif
#ifndef __noreturn
#define __noreturn      __attribute__((noreturn))
#endif
#ifndef __maybe_unused
#define __maybe_unused  __attribute__((unused))
#endif