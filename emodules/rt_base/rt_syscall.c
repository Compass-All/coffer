#include "rt_syscall.h"
#include "m3/page_pool.h"
#include "m3/page_table.h"
#include "rt_console.h"
#include <sys/stat.h>
#include <util/drv.h>
#include <util/memory.h>

uintptr_t usr_heap_top;

int rt_fstat(uintptr_t fd, uintptr_t sstat)
{
    struct stat* stat = (struct stat*)sstat;
    /* now only support stdio */
    if (fd > 4 || fd < 0) {
        return -1;
    }
    stat->st_dev = 26;
    stat->st_ino = 6;
    stat->st_nlink = 1;
    stat->st_mode = S_IWUSR | S_IRUSR | S_IRGRP;
    stat->st_uid = 1000;
    stat->st_gid = 5;
    stat->st_rdev = 34819;
    stat->st_size = 0;
    stat->st_blksize = 1024;
    stat->st_blocks = 0;
    return 0;
}

int rt_brk(uintptr_t addr)
{
    uintptr_t n_pages, ret = addr;
    if (addr == 0)
        return usr_heap_top;
    em_debug("####### brk start, usr_heap_top: 0x%lx########\n", usr_heap_top);
    em_debug("addr: 0x%lx\n", addr);
    if (usr_heap_top != PAGE_DOWN(usr_heap_top)) {
        em_error("usr_heap_top: 0x%llx not page aligned!\n", usr_heap_top);
    }
    addr = PAGE_UP(addr);
    if (addr > usr_heap_top) { // currently freeing does not work
        em_debug("ebi_brk cp 1\n");
        n_pages = (addr - usr_heap_top) >> EPAGE_SHIFT;
        em_debug("ebi_brk cp 2 n_pages = 0x%ld\n", n_pages);
        alloc_page(usr_heap_top, n_pages,
            PTE_U | PTE_R | PTE_W, IDX_USR);
        em_debug("ebi_brk cp 3\n");
    }
    usr_heap_top = addr;
    em_debug("####### brk end########\n");
    flush_tlb();
    return ret;
}

int rt_write(uintptr_t fd, uintptr_t content)
{
    // FIXME Workaround version
    uintptr_t content_pa = usr_get_pa(content);
    ecall_puts(content_pa);
    // char* str = (char*)content;
    // if (fd == 1) {
    //     while (*str) {
    //         ecall_putchar(*str);
    //         str++;
    //     }
    // }
    return 0;
}

int rt_close(uintptr_t fd)
{
    // TODO implementation
    return 0;
}

int rt_gettimeofday(struct timeval* tv, struct timezone* tz)
{
    // TODO implementation
    return 0;
}