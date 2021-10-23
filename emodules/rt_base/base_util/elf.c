#include "elf.h"

int elf_check(uintptr_t elf_addr, size_t elf_size)
{
    ehdr_t* ehdr = (ehdr_t*)elf_addr;
    unsigned char* ident = ehdr->e_ident;
    uintptr_t ph_end, sh_end;

    // Check headers
    if (ident[EI_MAG0] != ELFMAG0 || ident[EI_MAG1] != ELFMAG1 || ident[EI_MAG2] != ELFMAG2 || ident[EI_MAG3] != ELFMAG3) {
        // em_error("Not ELF file\n");
        goto error;
    }
    if (ident[EI_CLASS] != ELFCLASS) {
        // em_error("Not %d-bits file\n", __risv_xlen);
        goto error;
    }
    if (ehdr->e_type != ET_EXEC) {
        // em_error("Not executable file\n");
        goto error;
    }
    if (ehdr->e_machine != EM_RISCV) {
        // em_error("Invalid machine platform\n");
        goto error;
    }

    // Check program header table
    ph_end = ehdr->e_phoff + ehdr->e_phentsize * ehdr->e_phnum;
    if (elf_size < ph_end || ph_end < ehdr->e_phoff) {
        goto error;
    }

    // Check section table
    sh_end = ehdr->e_shoff + ehdr->e_shentsize * ehdr->e_shnum;
    if (elf_size < sh_end || sh_end < ehdr->e_shoff) {
        goto error;
    }

    return 0;

error:
    return 1;
}

int elf_map(uintptr_t elf_addr)
{
    ehdr_t* ehdr = (ehdr_t*)elf_addr;
    phdr_t* phdr = ehdr + ehdr->e_phoff;
    uintptr_t va_start, file_end, memory_end;
    uintptr_t va;
    unsigned int i;

    for (i = 0; i < ehdr->e_phnum; ++i) {
        if (phdr[i].p_type != PT_LOAD) {
            continue;
        }
        va_start = phdr[i].p_vaddr;
        file_end = va_start + phdr[i].p_filesz;
        memory_end = va_start + phdr[i].p_memsz;
        va = va_start;
    }
    return 0;
}