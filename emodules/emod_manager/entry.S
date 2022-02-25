.align 4
.section ".text.init"
.global _start

_start:
	la		sp, tmp_stack_top
	jal		emain