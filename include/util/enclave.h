#ifndef EBI_ENCLAVE_H
#define EBI_ENCLAVE_H

#include "util.h"
#include "drv.h"

#define PERI_NUM_MAX 128

#define NUM_ENCLAVE 180
#define NUM_CORES 10

#ifndef __ASSEMBLER__

typedef enum {
	ENC_FREE, // Unused/unloaded
	ENC_LOAD, // Loaded, but not started
	ENC_IDLE, // Waiting
	ENC_RUN	  // Running
} enclave_status_t;

typedef struct {
	uintptr_t id;

	uintptr_t ns_satp;
	uintptr_t ns_mepc;
	uintptr_t ns_mstatus;
	uintptr_t ns_medeleg;

	uintptr_t ns_sstatus;
	uintptr_t ns_stvec;
	uintptr_t ns_sscratch;
	uintptr_t ns_sie;
	uintptr_t ns_sepc;

	uintptr_t pa;
	uintptr_t mem_size;
	uintptr_t enclave_binary_size;
	uintptr_t drv_list;
	uintptr_t user_param;
	uintptr_t umode_context[MAX_INDEX];
	peri_addr_t peri_list[PERI_NUM_MAX];
	uint8_t peri_cnt;
	enclave_status_t status;

	uintptr_t pt_root_addr;
	// Inverse map for physical address
	uintptr_t inverse_map_addr;
	uintptr_t offset_addr;

	pmp_region pmp_reg[PMP_REGION_MAX];
} enclave_context_t;

extern enclave_context_t enclaves[NUM_ENCLAVE + 1];
extern int enclave_on_core[NUM_CORES];

extern void init_enclaves(void);
extern uintptr_t create_enclave(struct sbi_trap_regs *args, uintptr_t mepc);
extern uintptr_t enter_enclave(struct sbi_trap_regs *args, uintptr_t mepc);
extern uintptr_t exit_enclave(struct sbi_trap_regs *regs);
extern uintptr_t suspend_enclave(uintptr_t id, struct sbi_trap_regs *regs, uintptr_t mepc);
extern uintptr_t resume_enclave(uintptr_t id, struct sbi_trap_regs *regs);
enclave_context_t *eid_to_context(uintptr_t eid);
int enclave_num();
int check_alive(uintptr_t eid);

#endif // __ASSEMBLER__

#endif // EBI_ENCLAVE_H
