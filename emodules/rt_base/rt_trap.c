#include "rt_trap.h"
#include "m3/page_table.h"
#include "rt_console.h"
#include "rt_csr.h"
#include "rt_ecall.h"
#include "rt_syscall.h"
#include <stdint.h>
#include <util/util.h>

uintptr_t enclave_id;

void handle_interrupt(uintptr_t* regs, uintptr_t scause, uintptr_t sepc,
    uintptr_t stval)
{
    switch (scause) {
    case IRQ_S_TIMER:
        em_debug("IRQ_S_TIMER sepc=0x%08x, stval=0x%08x\n", sepc,
            stval);
        clear_csr(sip, SIP_STIP);
        break;
    case IRQ_S_SOFT:
        em_debug("IRQ_S_SOFT sepc=0x%08x, stval=0x%08x\n", sepc,
            stval);
        clear_csr(sip, SIP_SSIP);
        break;
    case IRQ_S_EXT:
        em_debug("IRQ_S_EXT sepc=0x%08x, stval=0x%08x\n", sepc, stval);
        clear_csr(sip, 1 << IRQ_S_EXT);
        clear_csr(sie, 1 << IRQ_S_EXT);
        break;
    default:
        em_error("Unknown interrupt %d! sepc=0x%llx, stval=0x%llx\n", scause, sepc, stval);
        ecall_exit_enclave(-1);
        __builtin_unreachable();
    }
}

void handle_exception(uintptr_t* regs, uintptr_t scause, uintptr_t sepc,
    uintptr_t stval)
{
    uintptr_t pa;
    em_error("Unhandled exception %lld! sepc=0x%llx, stval=0x%llx!\n", scause, sepc, stval);
    if (stval) {
        pa = usr_get_pa(stval);
        em_error("address access: va=0x%llx, pa=0x%llx\n", stval, pa);
    }
    // dump_umode_regs(regs);
    ecall_exit_enclave(-1);
    __builtin_unreachable();
}

void handle_syscall(uintptr_t* regs, uintptr_t scause, uintptr_t sepc,
    uintptr_t stval)
{
    uintptr_t sstatus = read_csr(sstatus);
    // sstatus |= SSTATUS_SUM;
    // write_csr(sstatus, sstatus);

    if (scause != CAUSE_USER_ECALL) {
        handle_exception(regs, scause, sepc, stval);
    }

    uintptr_t which = regs[CTX_INDEX_a7], arg_0 = regs[CTX_INDEX_a0],
              arg_1 = regs[CTX_INDEX_a1], retval = 0;
    switch (which) {
    case SYS_fstat:
        retval = rt_fstat(arg_0, arg_1);
        break;
    case SYS_write:
        em_debug("SYS_write\n");
        retval = rt_write(arg_0, arg_1);
        break;
    case SYS_close:
        retval = rt_close(arg_0);
        break;
    case SYS_brk:
        em_debug("SYS_brk: arg0 = 0x%lx\n", arg_0);
        retval = rt_brk(arg_0);
        em_debug("retval = 0x%lx\n", retval);
        break;
    case SYS_gettimeofday:
        retval = rt_gettimeofday((struct timeval*)arg_0, (struct timezone*)arg_1);
        break;
    case SYS_exit:
        em_debug("SYS_exit\n");
        ecall_exit_enclave(arg_0);
        __builtin_unreachable();
    default:
        em_error("syscall %d unimplemented!\n", which);
        ecall_exit_enclave(-1);
        __builtin_unreachable();
    }

    write_csr(sepc, sepc + 4);
    sstatus = sstatus & ~(SSTATUS_SPP | SSTATUS_UIE | SSTATUS_UPIE);
    write_csr(sstatus, sstatus);

    regs[CTX_INDEX_a0] = retval;
}