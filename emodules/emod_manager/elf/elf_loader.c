#include "elf_loader.h"
#include "../memory/page_table.h"
#include "../memory/page_pool.h"
#include "../panic/panic.h"
#include "../debug/debug.h"

// TODO: how to utilize the rest of the partition which
// the elf is located?

static u8 pte_flag_from_Elf64_word(Elf64_Word word)
{
	u8 flag = PTE_V | PTE_U;
	if (word & PF_R)
		flag |= PTE_R;
	if (word & PF_W)
		flag |= PTE_W;
	if (word & PF_X)
		flag |= PTE_X;

	return flag;
}

static int elf_check(Elf64_Ehdr *ehdr, usize elf_size)
{
	u8 *ident = ehdr->e_ident;
	vaddr_t phdr_end, shdr_end;

	// check headers
	if (ident[EI_MAG0] != ELFMAG0
	|| ident[EI_MAG1] !=  ELFMAG1
	|| ident[EI_MAG2] !=  ELFMAG2
	|| ident[EI_MAG3] !=  ELFMAG3) {
		panic("Invalid ELF!\n");
	}

	if (ident[EI_CLASS] != ELFCLASS64)
		panic("Not 64-bit ELF\n");
	
	if (ehdr->e_type != ET_EXEC)
		panic("Not executable file!\n");

	if (ehdr->e_machine != EM_RISCV)
		panic("Invalid machine platform\n");

	// check program header table
	phdr_end = ehdr->e_phoff + ehdr->e_phentsize * ehdr->e_phnum;
	if (elf_size < phdr_end || phdr_end < ehdr->e_phoff)
		panic("Program header error\n");
	
	// check section table
	shdr_end = ehdr->e_shoff + ehdr->e_shentsize * ehdr->e_shnum;
	if (elf_size < shdr_end || shdr_end < ehdr->e_shoff)
		panic("Section header error\n");

	return 0;
}

static void *memset(void* dst, int byte, size_t len)
{
    size_t offset = 0;
    uint64_t fill = byte;
    fill |= fill << 8;
    fill |= fill << 16;
    fill |= fill << 32;
    while (len > 0) {
        if (len > 8) {
            *((uint64_t*)(dst + offset)) = fill;
            offset += 8;
            len -= 8;
        } else if (len > 4) {
            *((uint32_t*)(dst + offset)) = fill & 0xFFFFFFFF;
            offset += 4;
            len -= 4;
        } else if (len > 2) {
            *((uint16_t*)(dst + offset)) = fill & 0xFFFF;
            offset += 2;
            len -= 2;
        } else {
            *((uint8_t*)(dst + offset)) = byte;
            ++offset;
            --len;
        }
    }

	return dst;
}

static int map_elf(paddr_t elf_paddr)
{
	vaddr_t elf_vaddr_linear = elf_paddr + linear_map_offset;

	Elf64_Ehdr *ehdr = (Elf64_Ehdr *)elf_vaddr_linear;
	Elf64_Phdr *phdr = (Elf64_Phdr *)(elf_vaddr_linear + ehdr->e_phoff);

	for (int i = 0; i < ehdr->e_phnum; i++) {
		if (phdr[i].p_type != PT_LOAD) {
			debug("Passing program header (i = %d)\n", i);
			continue;
		}
		debug("Loading program header (i = %d)\n", i);

		vaddr_t ph_vaddr	= phdr[i].p_vaddr;
		paddr_t ph_paddr	= elf_paddr + phdr[i].p_offset;
		usize file_size	= phdr[i].p_filesz;
		usize mem_size	= phdr[i].p_memsz;
		show(ph_vaddr); show(ph_paddr); show(file_size); show(mem_size);

		vaddr_t start_vaddr = PAGE_DOWN(ph_vaddr);
		vaddr_t end_vaddr 	= PAGE_UP(ph_vaddr + file_size);
		paddr_t start_paddr = PAGE_DOWN(ph_paddr);
		usize number_of_pages = (end_vaddr - start_vaddr) >> PAGE_SHIFT;
		show(start_vaddr); show(end_vaddr); show(start_paddr);
		show(number_of_pages);

		for (int j = 0; j < number_of_pages; j++) {
			map_page(
				start_vaddr + j * PAGE_SIZE,
				start_paddr + j * PAGE_SIZE,
				pte_flag_from_Elf64_word(phdr[i].p_flags),
				SV39_LEVEL_PAGE
			);
		}

		if (file_size < mem_size) { // need to allocate pages
			debug("Allocating more pages\n");
			// TODO: what if number_of_pages is larger than pool size?

			number_of_pages = 
				(PAGE_UP(mem_size) - PAGE_UP(file_size)) >> PAGE_SHIFT;
			paddr_t extra_page_paddr = alloc_umode_page(number_of_pages);

			show(number_of_pages);
			show(extra_page_paddr);

			for (int j = 0; j < number_of_pages; j++) {
				map_page(
					end_vaddr 			+ j * PAGE_SIZE,
					extra_page_paddr 	+ j * PAGE_SIZE,
					pte_flag_from_Elf64_word(phdr[i].p_flags),
					SV39_LEVEL_PAGE
				);
			}

			debug("zero mem-only portion\n");
			show(ph_vaddr + file_size);
			show(mem_size - file_size);
			memset((void *)(ph_vaddr + file_size), 0, mem_size - file_size);
		}
	}

	return 0;
}

// load elf and return entry point va
vaddr_t load_elf(paddr_t elf_paddr, usize elf_size)
{
	debug("Loading ELF\n");
	show(elf_paddr); show(elf_size);

	vaddr_t elf_vaddr_linear = elf_paddr + linear_map_offset;
	Elf64_Ehdr *ehdr = (Elf64_Ehdr *)elf_vaddr_linear;

	if (elf_check(ehdr, elf_size))
		panic("ELF Check Error\n");

	if (map_elf(elf_paddr))
		panic("ELF Map Error\n");

	return ehdr->e_entry;
}