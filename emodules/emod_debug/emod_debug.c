#include <emodules/emod_debug/emod_debug.h>
#include "dependency.h"
#include "printf.h"

// Emodule Debug Descriptor
static emod_desc_t emod_debug_desc = {
	.emod_id = EMODULE_ID_DEBUG,
	.name = "emodule debug",
	.__signature = 0
};
static emod_debug_api_t emod_debug_api;
static emod_debug_t emod_debug;

// Emodule Debug Functions

// printf from "printf.h"

static void hexdump(vaddr_t addr, usize len)
{
	const u8 group_size = 4;
	u32 *ptr = (u32 *)addr;
	for (int i = 0; i < len; i += group_size * sizeof(u32),
		ptr += group_size) {

		printf("[hexdump] 0x%p:\t", ptr);
		for (int j = 0; j < group_size - 1; j++)
			printf("0x%08x\t", ptr[j]);
		printf("0x%08x\n", ptr[group_size - 1]);
	}
}

static void assert(u8 *ptr1, u8 *ptr2, usize len)
{
	if (!ptr1 || !ptr2) {
		printf("NULL Pointer during assertion\n");
	}
	for (int i = 0; i < len; i++) {
		if (*ptr1 != *ptr2) {
			printf("[assert] 0x%x\t(at 0x%p)!= 0x%x\t(at 0x%p)",
				*ptr1, ptr1, *ptr2, ptr2);
			printf("[assert] Assert failed\n");
		}
	}
	printf("[assert] Assert passed!\n");
}

// Emodule Init and Getter
static emod_debug_t get_emodule()
{
	return emod_debug;
}

__attribute__((section(".text.init")))
vaddr_t debug_init(vaddr_t emod_manager_getter)
{
	emod_debug_api = (emod_debug_api_t) {
		.printd = printf,
		.hexdump = hexdump,
		.assert = assert
	};

	emod_debug = (emod_debug_t) {
		.emod_debug_desc = emod_debug_desc,
		.emod_debug_api = emod_debug_api
	};

	printf("[debug_init] emod_manager_getter at 0x%lx\n",
		emod_manager_getter);

	if (emod_manager_getter == (vaddr_t)0UL) {
		printf("Error: NULL emod_manager getter\n");
		return (vaddr_t)0UL;
	}

	emod_manager_t (*getter)(void)
		= (void *)emod_manager_getter;
	emod_manager = getter();

	return (vaddr_t)get_emodule;
}