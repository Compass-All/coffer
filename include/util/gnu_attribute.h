#pragma once

#include <memory/page_table.h>
#include <memory/memory.h>

#define __unused		__attribute__((unused))
#define __diverge		__attribute__((noreturn))
#define __page_aligned	__attribute__((aligned(PAGE_SIZE)))