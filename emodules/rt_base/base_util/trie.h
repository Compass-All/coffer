
#pragma once

#include <util/memory.h>

typedef struct trie {
    uint32_t next[PAGE_DIR_POOL][512], cnt;
} trie_t;