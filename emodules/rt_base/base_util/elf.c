#include "elf.h"
#include "../m3/page_pool.h"
#include "../m3/page_table.h"
#include "../rt_console.h"

static pte_attr_t get_pte_attr(word_t pflags)
{
    uintptr_t res = PTE_V;
    if (pflags & PF_R)
        res |= PTE_R;
    if (pflags & PF_W)
        res |= PTE_W;
    if (pflags & PF_X)
        res |= PTE_X;
    return res;
}

static int elf_check(ehdr_t* ehdr, size_t elf_size)
{
    unsigned char* ident = ehdr->e_ident;
    uintptr_t ph_end, sh_end;

    // Check headers
    if (ident[EI_MAG0] != ELFMAG0 || ident[EI_MAG1] != ELFMAG1 || ident[EI_MAG2] != ELFMAG2 || ident[EI_MAG3] != ELFMAG3) {
        em_error("Not ELF file\n");
        goto error;
    }
    if (ident[EI_CLASS] != ELFCLASS) {
        em_error("Not %d-bits file\n", __riscv_xlen);
        goto error;
    }
    if (ehdr->e_type != ET_EXEC) {
        em_error("Not executable file\n");
        goto error;
    }
    if (ehdr->e_machine != EM_RISCV) {
        em_error("Invalid machine platform\n");
        goto error;
    }

    // Check program header table
    ph_end = ehdr->e_phoff + ehdr->e_phentsize * ehdr->e_phnum;
    if (elf_size < ph_end || ph_end < ehdr->e_phoff) {
        em_error("Program header error\n");
        goto error;
    }

    // Check section table
    sh_end = ehdr->e_shoff + ehdr->e_shentsize * ehdr->e_shnum;
    if (elf_size < sh_end || sh_end < ehdr->e_shoff) {
        em_error("Section header error\n");
        goto error;
    }

    return 0;

error:
    return 1;
}

static int elf_map(uintptr_t elf_addr)
{
    ehdr_t* ehdr = (ehdr_t*)elf_addr;
    phdr_t* phdr = (phdr_t*)(elf_addr + ehdr->e_phoff);
    uintptr_t va_start, pa_start, file_end;
    size_t n_pages;
    unsigned int i;

    for (i = 0; i < ehdr->e_phnum; ++i) {
        if (phdr[i].p_type != PT_LOAD) {
            continue;
        }
        em_debug("Loading ph#%u\n", i);
        va_start = phdr[i].p_vaddr;
        pa_start = elf_addr + phdr[i].p_offset;
        file_end = va_start + phdr[i].p_filesz;
        n_pages = (PAGE_UP(file_end) - PAGE_DOWN(va_start)) >> EPAGE_SHIFT;
        em_debug("va_start=%p, file_end=%p, n_pages=%llu, p_flags = %d\n", (void*)va_start, (void*)file_end, n_pages, phdr[i].p_flags);
        map_page(PAGE_DOWN(va_start), PAGE_DOWN(pa_start), n_pages,
            PTE_U | get_pte_attr(phdr[i].p_flags), 1);
    }
    return 0;
}

uintptr_t elf_load(uintptr_t elf_addr, size_t elf_size, uintptr_t* usr_heap_top_addr)
{
    em_debug("Loading ELF at %p with size=%d\n", (void*)elf_addr, elf_size);
    ehdr_t* ehdr = (ehdr_t*)elf_addr;
    if (elf_check(ehdr, elf_size)) {
        em_error("ELF check failed\n");
        return -1;
    }
    if (elf_map(elf_addr)) {
        em_error("ELF mapping failed\n");
        return -1;
    }
    *usr_heap_top_addr = EUSR_HEAP_START;
    return ehdr->e_entry;
}

void elf_load_finalize(uintptr_t elf_va)
{
    ehdr_t* ehdr = (ehdr_t*)elf_va;
    phdr_t* phdr = (phdr_t*)(elf_va + ehdr->e_phoff);
    uintptr_t va_start, file_end, memory_end;
    size_t n_pages;
    unsigned int i;

    for (i = 0; i < ehdr->e_phnum; ++i) {
        if (phdr[i].p_type != PT_LOAD) {
            continue;
        }
        em_debug("Loading ph#%u\n", i);
        va_start = phdr[i].p_vaddr;
        file_end = PAGE_UP(va_start + phdr[i].p_filesz);
        memory_end = PAGE_UP(va_start + phdr[i].p_memsz);
        em_debug("file_end=%p, memory_end=%p\n", (void*)file_end, (void*)memory_end);
        if (file_end < memory_end) {
            n_pages = (memory_end - file_end) >> EPAGE_SHIFT;
            em_debug("file_end=%p, memory_end=%p, n_pages=%llu\n", (void*)file_end, (void*)memory_end, n_pages);
            alloc_page(file_end, n_pages, PTE_U | get_pte_attr(phdr[i].p_flags), IDX_USR);
        }
    }
    em_debug("End\n");
}