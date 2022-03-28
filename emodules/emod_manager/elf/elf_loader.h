#pragma once 

#include <elf.h>
#include <types.h>

vaddr_t load_elf(paddr_t elf_paddr, usize elf_size);