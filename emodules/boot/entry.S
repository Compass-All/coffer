.align 4
.section ".text.init"
.global _start

_start:
	li	a0, 'h'
	li	a7, 1
	ecall
	li	a0, 'i'
	li	a7, 1
	ecall
	li	a0, '\n'
	li	a7, 1
	ecall