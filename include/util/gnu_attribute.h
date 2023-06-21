#pragma once

#define __unused		__attribute__((unused))
#define __diverge		__attribute__((noreturn))
#define __page_aligned	__attribute__((aligned(PAGE_SIZE)))
#define __packed        __attribute__((packed))
#define __noreturn      __attribute__((noreturn))

#ifndef __maybe_unused
#define __maybe_unused  __attribute__((unused))
#endif