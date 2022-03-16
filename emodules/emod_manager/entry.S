#include <util/register.h>

.align 4
.section ".text.init"
.global _start
_start:
	la		sp, tmp_stack_top
	jal		emain_upper_half

    la      s7, temp_stvec
    add     s7, s6, s7
    csrw    stvec, s7

    mv      sp, a1
    sfence.vma
    csrw    satp, a0
.align 2
temp_stvec:
    sfence.vma

    la      s7, trap_handler
    csrw    stvec, s7

    jal     emain_lower_half

dump_reg:
    li      a7, 0x19260817
    li      a6, 499 // debug, show a0 ~ a3
    ecall
    ret

.macro RESTORE_CONTEXT
    LOAD ra,  EAPP_CTX_REG_OFFSET(ra)(sp)
    LOAD gp,  EAPP_CTX_REG_OFFSET(gp)(sp)
    LOAD tp,  EAPP_CTX_REG_OFFSET(tp)(sp)
    LOAD t0,  EAPP_CTX_REG_OFFSET(t0)(sp)
    LOAD t1,  EAPP_CTX_REG_OFFSET(t1)(sp)
    LOAD t2,  EAPP_CTX_REG_OFFSET(t2)(sp)
    LOAD s0,  EAPP_CTX_REG_OFFSET(s0)(sp)
    LOAD s1,  EAPP_CTX_REG_OFFSET(s1)(sp)
    LOAD a0,  EAPP_CTX_REG_OFFSET(a0)(sp)
    LOAD a1,  EAPP_CTX_REG_OFFSET(a1)(sp)
    LOAD a2,  EAPP_CTX_REG_OFFSET(a2)(sp)
    LOAD a3,  EAPP_CTX_REG_OFFSET(a3)(sp)
    LOAD a4,  EAPP_CTX_REG_OFFSET(a4)(sp)
    LOAD a5,  EAPP_CTX_REG_OFFSET(a5)(sp)
    LOAD a6,  EAPP_CTX_REG_OFFSET(a6)(sp)
    LOAD a7,  EAPP_CTX_REG_OFFSET(a7)(sp)
    LOAD s2,  EAPP_CTX_REG_OFFSET(s2)(sp)
    LOAD s3,  EAPP_CTX_REG_OFFSET(s3)(sp)
    LOAD s4,  EAPP_CTX_REG_OFFSET(s4)(sp)
    LOAD s5,  EAPP_CTX_REG_OFFSET(s5)(sp)
    LOAD s6,  EAPP_CTX_REG_OFFSET(s6)(sp)
    LOAD s7,  EAPP_CTX_REG_OFFSET(s7)(sp)
    LOAD s8,  EAPP_CTX_REG_OFFSET(s8)(sp)
    LOAD s9,  EAPP_CTX_REG_OFFSET(s9)(sp)
    LOAD s10, EAPP_CTX_REG_OFFSET(s10)(sp)
    LOAD s11, EAPP_CTX_REG_OFFSET(s11)(sp)
    LOAD t3,  EAPP_CTX_REG_OFFSET(t3)(sp)
    LOAD t4,  EAPP_CTX_REG_OFFSET(t4)(sp)
    LOAD t5,  EAPP_CTX_REG_OFFSET(t5)(sp)
    LOAD t6,  EAPP_CTX_REG_OFFSET(t6)(sp)
    addi    sp, sp, EAPP_CTX_SIZE
.endm

.macro SAVE_CONTEXT
    addi sp, sp, -EAPP_CTX_SIZE
    STORE ra,  EAPP_CTX_REG_OFFSET(ra)(sp)
    STORE gp,  EAPP_CTX_REG_OFFSET(gp)(sp)
    STORE tp,  EAPP_CTX_REG_OFFSET(tp)(sp)
    STORE t0,  EAPP_CTX_REG_OFFSET(t0)(sp)
    STORE t1,  EAPP_CTX_REG_OFFSET(t1)(sp)
    STORE t2,  EAPP_CTX_REG_OFFSET(t2)(sp)
    STORE s0,  EAPP_CTX_REG_OFFSET(s0)(sp)
    STORE s1,  EAPP_CTX_REG_OFFSET(s1)(sp)
    STORE a0,  EAPP_CTX_REG_OFFSET(a0)(sp)
    STORE a1,  EAPP_CTX_REG_OFFSET(a1)(sp)
    STORE a2,  EAPP_CTX_REG_OFFSET(a2)(sp)
    STORE a3,  EAPP_CTX_REG_OFFSET(a3)(sp)
    STORE a4,  EAPP_CTX_REG_OFFSET(a4)(sp)
    STORE a5,  EAPP_CTX_REG_OFFSET(a5)(sp)
    STORE a6,  EAPP_CTX_REG_OFFSET(a6)(sp)
    STORE a7,  EAPP_CTX_REG_OFFSET(a7)(sp)
    STORE s2,  EAPP_CTX_REG_OFFSET(s2)(sp)
    STORE s3,  EAPP_CTX_REG_OFFSET(s3)(sp)
    STORE s4,  EAPP_CTX_REG_OFFSET(s4)(sp)
    STORE s5,  EAPP_CTX_REG_OFFSET(s5)(sp)
    STORE s6,  EAPP_CTX_REG_OFFSET(s6)(sp)
    STORE s7,  EAPP_CTX_REG_OFFSET(s7)(sp)
    STORE s8,  EAPP_CTX_REG_OFFSET(s8)(sp)
    STORE s9,  EAPP_CTX_REG_OFFSET(s9)(sp)
    STORE s10, EAPP_CTX_REG_OFFSET(s10)(sp)
    STORE s11, EAPP_CTX_REG_OFFSET(s11)(sp)
    STORE t3,  EAPP_CTX_REG_OFFSET(t3)(sp)
    STORE t4,  EAPP_CTX_REG_OFFSET(t4)(sp)
    STORE t5,  EAPP_CTX_REG_OFFSET(t5)(sp)
    STORE t6,  EAPP_CTX_REG_OFFSET(t6)(sp)
.endm

.align 4
.section ".text"
.global trap_handler
trap_handler:
	csrrw	sp, sscratch, sp // swap sp and sscratch
	// if sp is zero, this is a kernel trap, jump to 1f
	bnez    sp, 1f
	// sscratch is not zero, user trap
    csrr    sp, sscratch
1:
	SAVE_CONTEXT
	csrrw	t1, sscratch, x0	// t1 = user sp, sscratch = 0
	STORE	t1, EAPP_CTX_REG_OFFSET(sp)(sp) // save user sp

	jal		exception_handler

	LOAD	t1, EAPP_CTX_REG_OFFSET(sp)(sp) // restore user sp
	csrw	sscratch, t1
	RESTORE_CONTEXT

	csrrw	sp, sscratch, sp
	sret