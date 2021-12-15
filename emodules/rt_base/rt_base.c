#include "base_util/elf.h"
#include "base_util/md2.h"
#include "base_util/memory.h"
#include "m3/inv_map.h"
#include "m3/page_pool.h"
#include "m3/page_table.h"
#include "rt_console.h"
#include "rt_csr.h"
#include "rt_ecall.h"
#include <util/drv.h>
#include <util/memory.h>
#include <util/util.h>

extern uintptr_t enclave_id;
extern uintptr_t usr_heap_top;
extern uintptr_t va_top;

#define PUSH(usr_sp, val)               \
    do {                                \
        (usr_sp) -= __SIZEOF_POINTER__; \
        *(uintptr_t*)(usr_sp) = val;    \
    } while (0)

#define __va(x) usr_get_pa((x) + enc_va_pa_offset)

#define MAP_BASE_SECTION(sec_name, pte_flags)                                       \
    do {                                                                            \
        extern char _##sec_name##_start, _##sec_name##_end;                         \
        uintptr_t sec_name##_start = PAGE_DOWN((uintptr_t)&_##sec_name##_start);    \
        uintptr_t sec_name##_end = (uintptr_t)&_##sec_name##_end;                   \
        uintptr_t sec_name##_size = sec_name##_end - sec_name##_start;              \
        size_t n_base_##sec_name##_pages = PAGE_UP(sec_name##_size) >> EPAGE_SHIFT; \
        map_page(enc_va_pa_offset + sec_name##_start,                               \
            sec_name##_start, n_base_##sec_name##_pages,                            \
            pte_flags, 0);                                                          \
        em_debug(#sec_name ": 0x%x - 0x%x -> 0x%x\n",                               \
            sec_name##_start, sec_name##_end,                                       \
            __va(sec_name##_start));                                                \
    } while (0)

#define RT_BASE_STACK_SIZE 0x1000

uint8_t rt_base_stack[RT_BASE_STACK_SIZE];

void* const rt_base_stack_top = (void*)rt_base_stack + RT_BASE_STACK_SIZE;

static uintptr_t init_usr_stack(uintptr_t usr_sp, uintptr_t argc)
{
    PUSH(usr_sp, argc);
    return usr_sp;
}

static inline void attest_payload(void* payload_start, size_t payload_size)
{
    uint8_t md2hash[MD2_BLOCK_SIZE];
    int i;

    md2(payload_start, payload_size, md2hash);
    em_debug("MD2:\n");
    for (i = 0; i < MD2_BLOCK_SIZE; ++i) {
        em_debug("%x\n", md2hash[i]);
    }
}

// Memory mapping setup
static void init_map_alloc_pages(drv_addr_t* drv_list,
    uintptr_t page_table_start, size_t page_table_size,
    uintptr_t usr_avail_start, size_t usr_avail_size,
    uintptr_t base_avail_start, size_t base_avail_size)
{
    uintptr_t usr_stack_start;
    size_t n_usr_stack_pages, n_base_stack_pages;
    uintptr_t drv_sp;

    // Remaining user memory
    map_page(enc_va_pa_offset + usr_avail_start, usr_avail_start,
        usr_avail_size >> EPAGE_SHIFT, PTE_V | PTE_W | PTE_R, 0);
    em_debug("usr.remain: 0x%x - 0x%x -> 0x%x\n", usr_avail_start,
        usr_avail_start + PAGE_DOWN(usr_avail_size),
        __va(usr_avail_start));

    // Allocate user stack (r/w)
    n_usr_stack_pages = (PAGE_UP(EUSR_STACK_SIZE) >> EPAGE_SHIFT) + 1;
    em_debug("User stack needs %d pages\n", n_usr_stack_pages);
    usr_stack_start = EUSR_STACK_START;
    // uintptr_t usr_stack_end = EUSR_STACK_TOP; // debug use
    // em_debug("User stack: 0x%lx - 0x%lx\n", usr_stack_start, usr_stack_end);
    alloc_page(usr_stack_start, n_usr_stack_pages,
        PTE_V | PTE_W | PTE_R | PTE_U, IDX_USR);

    // Map pages for base module
    // `.text' section
    MAP_BASE_SECTION(text, PTE_V | PTE_X | PTE_R);
    // Page table (and trie)
    map_page(enc_va_pa_offset + page_table_start, page_table_start,
        (page_table_size) >> EPAGE_SHIFT, PTE_V | PTE_W | PTE_R, 0);
    em_debug("page_table and trie: 0x%x - 0x%x -> 0x%x\n", page_table_start,
        page_table_start + page_table_size, __va(page_table_start));
    // `.rodata', `.bss', `.init.data`, `.data' sections
    MAP_BASE_SECTION(rodata, PTE_V | PTE_R);
    MAP_BASE_SECTION(srodata, PTE_V | PTE_R);
    MAP_BASE_SECTION(bss, PTE_V | PTE_W | PTE_R);
    MAP_BASE_SECTION(init_data, PTE_V | PTE_W | PTE_R);
    MAP_BASE_SECTION(data, PTE_V | PTE_W | PTE_R);
    // Remaining base memory
    map_page(enc_va_pa_offset + base_avail_start, base_avail_start,
        PAGE_DOWN(base_avail_size) >> EPAGE_SHIFT,
        PTE_V | PTE_W | PTE_R, 0);
    em_debug("drv.remain: 0x%x - 0x%x -> 0x%x\n", base_avail_start,
        base_avail_start + PAGE_DOWN(base_avail_size),
        __va(base_avail_start));

    // Allocate base stack (r/w)
    n_base_stack_pages = (PAGE_UP(ERT_STACK_SIZE)) >> EPAGE_SHIFT;
    em_debug("drv stack uses %d pages\n", n_base_stack_pages);
    drv_sp = ERT_STACK_TOP - ERT_STACK_SIZE;
    uintptr_t stack_pa = alloc_page(drv_sp, n_base_stack_pages, PTE_V | PTE_W | PTE_R, IDX_RT);
    em_debug("stack_pa@0x%lx\n", stack_pa);
    drv_sp += ERT_STACK_SIZE;
    em_debug("sp: 0x%lx\nPage table root: 0x%lx\n", drv_sp,
        get_pt_root());
}

/* Initialize memory for driver, including stack, heap, page table.
 * Memory layout:
 *
 *                     -----------------  HIGH ADDR
 *                       < base memory space >
 * base_avail_start => -----------------
 *                     ^ < trie >
 *                     | trie_size == PAGE_UP(sizeof(trie_t))
 *                     v
 * trie_start =======> -----------------
 *                     ^ < page table >
 *                     | page_table_size == PAGE_UP(PAGE_DIR_POOL * EPAGE_SIZE)
 *                     v
 * page_table_start => -----------------
 *                     ^ < driver list >
 *                     | ~ DRV_MAX * sizeof(drv_addr_t)
 *                     v
 * drv_list =========> -----------------
 *                       < user memory space >
 * usr_avail_start ==> -----------------
 *                     ^ < user payload >
 *                     | PAGE_UP(payload_size)
 *                     v
 * payload_pa_start => -----------------  LOW ADDR
 *
 */

/* WIP *
void _init_mem(uintptr_t base_pa_start, uintptr_t argv)
{
    uintptr_t* ptr = (uintptr_t*)base_pa_start;
    uintptr_t usr_pa_start = base_pa_start + PARTITION_SIZE;
    uintptr_t argc;
    uintptr_t satp, sstatus, sepc;
    uintptr_t pa;
    size_t base_size, payload_size;

    enclave_id = *ptr++;
    base_size = *ptr++;
    payload_size = *ptr++;
    argc = *ptr++;
    attest_payload((void*)usr_pa_start, payload_size);

    // Initialize base page pool
    va_top = ERT_VA_START;
    enc_va_pa_offset = ERT_VA_START - base_pa_start;
    pa = PAGE_UP((uintptr_t)ptr + base_size);
    pt_root_pa = pa;
    pa += PAGE_UP(PAGE_DIR_POOL * EPAGE_SIZE) + PAGE_UP(sizeof(trie_t));
    page_pool_init(pa, PAGE_DOWN(usr_pa_start - pa), IDX_RT);

    // Initialize user page pool
    pa = usr_pa_start + PAGE_UP(payload_size);
    page_pool_init(pa, PAGE_DOWN(PARTITION_UP(pa) - pa), IDX_USR);
    sepc = elf_load(usr_pa_start, payload_size, &usr_heap_top);
    if (sepc == -1) {
        em_error("ELF load failed!\n");
        ecall_exit_enclave(-1);
        __builtin_unreachable();
    }

    // init_mem_alloc_pages

    satp = (uintptr_t)get_pt_root() >> EPAGE_SHIFT;
    satp |= (uintptr_t)SATP_MODE_SV39 << SATP_MODE_SHIFT;
    sstatus = read_csr(sstatus);
    sstatus |= SSTATUS_SUM;
    write_csr(sstatus, sstatus);

    // Inform M-mode of locations of page table and inverse mapping
    ecall_map_register(&pt_root_pa, &inv_map, &enc_va_pa_offset);
    va_top += PARTITION_SIZE;

    asm volatile("fence rw, rw");
    flush_tlb();
    em_debug("End of init_mem\n");

    asm volatile("mv a0, %0" ::"r"(satp));
    asm volatile("mv a1, %0" ::"r"(ERT_STACK_TOP));
    asm volatile("mv a2, %0" ::"r"(sepc));
    asm volatile("mv a3, %0" ::"r"(EUSR_STACK_TOP));
    asm volatile("mv a4, %0" ::"r"(argc));
}
//*/

void init_mem(uintptr_t base_pa_start, uintptr_t id, uintptr_t payload_pa_start,
    uintptr_t payload_size, drv_addr_t drv_list[MAX_DRV],
    uintptr_t argc, uintptr_t argv)
{
    uintptr_t page_table_start, page_table_size;
    uintptr_t trie_start, trie_size;
    uintptr_t base_avail_start, base_avail_size;
    uintptr_t usr_avail_start, usr_avail_size;
    uintptr_t satp, sstatus;
    uintptr_t usr_pc;
    uintptr_t usr_sp_pa;
    uintptr_t usr_sp = EUSR_STACK_TOP;
    uintptr_t drv_sp = ERT_STACK_TOP;

    // Update VA/PA offset
    enc_va_pa_offset = ERT_VA_START - base_pa_start;
    em_debug("\033[1;33moffset: 0x%lx\n\033[0m", enc_va_pa_offset);
    // `va_top' will increase by EMEM_SIZE after `page_pool_init'
    va_top = ERT_VA_START;
    attest_payload((void*)payload_pa_start, payload_size);
    enclave_id = id;

    // Setup page table, trie and base memory space
    page_table_start = PAGE_UP((uintptr_t)drv_list /* + MAX_DRV * sizeof(drv_addr_t)*/);
    page_table_size = PAGE_UP(PAGE_DIR_POOL * EPAGE_SIZE);
    em_debug("page_table_start = 0x%llx\n", page_table_start);
    em_debug("page_table_size = 0x%llx\n", page_table_size);
    pt_root_pa = page_table_start;
    memset((void*)page_table_start, 0, page_table_size);

    trie_start = page_table_start + page_table_size;
    trie_size = PAGE_UP(sizeof(trie_t));
    memset((void*)trie_start, 0, trie_size);

    base_avail_start = trie_start + trie_size;
    base_avail_size = PAGE_DOWN(PARTITION_UP(base_avail_start) - base_avail_start);
    em_debug("base_avail_start = 0x%lx\n", base_avail_start);
    em_debug("base_avail_size = %lx\n", base_avail_size);
    page_pool_init(base_avail_start, base_pa_start, base_avail_size, IDX_RT);

    // Setup user memory space
    usr_avail_start = PAGE_UP(payload_pa_start + payload_size);
    usr_avail_size = PAGE_DOWN(PARTITION_SIZE - payload_size % PARTITION_SIZE);
    page_pool_init(usr_avail_start, base_pa_start, usr_avail_size, IDX_USR);
    em_debug("Initializing user page pool: 0x%llx, size: 0x%llx\n",
        usr_avail_start, usr_avail_size);
    em_debug("User page pool initialization done\n");
    // check_pte_all_zero(); // Is it necessary?
    em_debug("\033[1;33mroot: 0x%llx\n\033[0m", get_pt_root());
    va_top += PARTITION_UP(usr_avail_start - base_pa_start);

    inv_map_init();

    usr_pc = elf_load(payload_pa_start, payload_size, &usr_heap_top);
    if (usr_pc == -1) {
        em_error("ELF load error\n");
    } else {
        em_debug("ELF load done. sepc=0x%lx\n", usr_pc);
    }

    init_map_alloc_pages(drv_list, page_table_start,
        page_table_size + trie_size, usr_avail_start,
        usr_avail_size, base_avail_start, base_avail_size);
    usr_sp_pa = usr_get_pa(usr_sp);

    // Update `satp', `sstatus', allow S-mode access to U-mode memory
    em_debug("usr sp: 0x%llx\n", usr_sp);
    satp = (uintptr_t)get_pt_root() >> EPAGE_SHIFT;
    satp |= (uintptr_t)SATP_MODE_SV39 << SATP_MODE_SHIFT;
    sstatus = read_csr(sstatus);
    sstatus |= SSTATUS_SUM;
    write_csr(sstatus, sstatus);

    // Inform M-mode of locations of page table and inverse mapping
    ecall_map_register(&pt_root_pa, &inv_map, &enc_va_pa_offset);
    em_debug("After ecall map_register\n");

    asm volatile("fence rw, rw");
    flush_tlb();
    em_debug("End of init_mem\n");

    asm volatile("mv a0, %0" ::"r"(satp));
    asm volatile("mv a1, %0" ::"r"(drv_sp));
    asm volatile("mv a2, %0" ::"r"(usr_pc));
    asm volatile("mv a3, %0" ::"r"(usr_sp));
    asm volatile("mv a4, %0" ::"r"(usr_sp_pa));
}

// Below code is invoked after `satp' configuration.

void prepare_boot(uintptr_t argc, uintptr_t argv, uintptr_t usr_pc, uintptr_t usr_sp)
{
    em_debug("Begin prepare_boot: argc=%lu, argv=%p\n", argc, (void*)argv);
    // Allow S-mode to access U-mode memory
    uintptr_t sstatus = read_csr(sstatus);
    sstatus |= SSTATUS_SUM;
    sstatus &= ~SSTATUS_SPP;
    write_csr(sstatus, sstatus);
    usr_sp = init_usr_stack(usr_sp, argc);
    em_debug("After init usr_stack\n");

    // Allow S-mode traps/interrupts
    write_csr(sie, SIE_SEIE | SIE_SSIE);

    em_debug("End of prepare_boot\n");

    // Set U-mode entry
    write_csr(sepc, usr_pc);
    write_csr(sscratch, usr_sp);
}

#ifdef EMODULES_DEBUG
void show_csr(void)
{
    uintptr_t sstatus, sepc, stvec, satp;
    em_debug("------------------\n");
    sstatus = read_csr(sstatus);
    sepc = read_csr(sepc);
    stvec = read_csr(stvec);
    satp = read_csr(satp);

    em_debug("status: 0x%lx\n", sstatus);
    em_debug("sepc: 0x%lx\n", sepc);
    em_debug("stvec: 0x%lx\n", stvec);
    em_debug("satp: 0x%lx\n", satp);
    em_debug("------------------\n");
}
#endif