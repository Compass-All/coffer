#pragma once

#include_next <elf.h>
#include <stddef.h>

#if __riscv_xlen == 64
typedef Elf64_Ehdr ehdr_t;
typedef Elf64_Phdr phdr_t;
typedef Elf64_Shdr shdr_t;
typedef Elf64_Word word_t;
#define ELFCLASS ELFCLASS64
#elif __riscv_xlen == 32
typedef Elf32_Ehdr ehdr_t;
typedef Elf32_Phdr phdr_t;
typedef Elf32_Shdr shdr_t;
typedef Elf32_Word word_t;
#define ELFCLASS ELFCLASS32
#else
#error "Unexpected __riscv_xlen"
#endif

uintptr_t elf_load(uintptr_t elf_addr, size_t elf_size);