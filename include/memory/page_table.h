#pragma once

#include <types.h>

#define SV39_LEVEL_PAGE	2
#define SV39_LEVEL_MEGA	1
#define SV39_LEVEL_GIGA	0

#define SV39_VPN_LEN	9

typedef struct {
	u64	ppn:	44;
	u64	asid:	16;
#define SATP_BARE	0
#define SATP_SV39	8
#define SATP_SV48	9
	u64 mode:	4;
} csr_satp_t;

#define PTE_V	(1U << 0)
#define PTE_R	(1U << 1)
#define PTE_W	(1U << 2)
#define PTE_X	(1U << 3)
#define PTE_U	(1U << 4)
#define PTE_G	(1U << 5)
#define PTE_A	(1U << 6)
#define PTE_D	(1U << 7)
/**
 * @brief Sv39 page table entry
 * 
 * @note The A and D bits can be managed in a simple scheme:
 * when a virtual page is accessed and A bit is clear, or
 * is written when D bit is clear, a *page-fault exception*
 * is raised.
 */
typedef struct {
	u64	v:	1;	// valid
	u64	r:	1;	// read
	u64	w:	1;	// write
	u64	x:	1;	// execute
	u64	u:	1;	// user mode
	u64	g:	1;	// global
	u64	a:	1;	// accessed
	u64	d:	1;	// dirty

	u64	__rsw0:	2;

	u64	ppn:	44;

	u64	__rsw1:	10;
} pte_t;

/**
 * @brief Sv39 Virtual Address
 * 
 * @note Field *extension* is important. It must be
 * all clear or all set according to the most significant
 * bit in vpn2 (bit 38).
 */
typedef struct {
	u64	offset: 12;
	u64	vpn0: 9;
	u64	vpn1: 9;
	u64 vpn2: 9;
	u64 extension:	25;
} sv39_vaddr_t;

typedef struct {
	u64	offset: 12;
	u64	ppn:	44;
	// u64	ppn0: 9;
	// u64	ppn1: 9;
	// u64	ppn2: 26;
	u64	__rsw:	8;
} sv39_paddr_t;

static inline u64 get_vpn(sv39_vaddr_t va, u8 level)
{
	switch (level)
	{
	case 0:
		return va.vpn0;
	
	case 1:
		return va.vpn1;

	case 2:
		return va.vpn2;

	default:
		return 0UL;
	}
}

// cast vaddr_t to sv39_vaddr_t
static inline sv39_vaddr_t va_to_sv39(vaddr_t va)
{
	s64 extended_va = (s64)(va << 25) >> 25;
	return *(sv39_vaddr_t *)&extended_va;
}

static inline vaddr_t sv39_to_va(sv39_vaddr_t sv39_va)
{
	return *(vaddr_t *)&sv39_va;
}

static inline sv39_paddr_t pa_to_sv39(paddr_t pa)
{
	return *(sv39_paddr_t *)&pa;
}

static inline paddr_t sv39_to_pa(sv39_paddr_t sv39_pa)
{
	return *(paddr_t *)&sv39_pa;
}

static inline void flush_tlb()
{
	asm volatile("sfence.vma");
}