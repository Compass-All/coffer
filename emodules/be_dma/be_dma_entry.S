#include <ebi_ecall.h>

	.section ".text.init"
    .global trap_handler
    .align 4
trap_handler:
    csrrw   sp, sscratch, sp
    /* Is this a kernel trap? */
    bnez    sp, 0f
    csrr    sp, sscratch
0: 
    /* C Call Convention */
    csrr    a1, scause
    mv      a0, sp
    csrr    a2, sepc
    csrr    a3, stval

    /* Is this a interrupt? */
    bgez    a1, 1f

    /* Clear highest bit */
    slli    a1, a1, 1
    srli    a1, a1, 1
    jal     be_interrupt_entry
    j       ret_to_sm
1:  
    jal     be_entry
    j       ret_to_sm

ret_to_sm:
	# to do: implement backend ret ecall
	li a7, SBI_EXT_EBI
	li a6, SBI_EXT_EBI_BACK_FROM_BACKEND
	ecall