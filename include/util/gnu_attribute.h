#pragma once

#ifndef PAGE_SIZE
#define PAGE_SIZE	(1UL << 12)
#endif

#define __unused		__attribute__((unused))
#define __diverge		__attribute__((noreturn))
#define __page_aligned	__attribute__((aligned(PAGE_SIZE)))