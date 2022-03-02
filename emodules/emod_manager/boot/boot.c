#include <types.h>
#include "../printf/printf.h"
#include "../printf/debug.h"
#include <emodules/emod_manager/emod_manager.h>
#include <ebi_ecall.h>

#define TMP_STACK_SIZE	0x1000

u8 tmp_stack[TMP_STACK_SIZE];
void *const tmp_stack_top = (void *)tmp_stack + TMP_STACK_SIZE;

#define SBI_ECALL(__extid, __funid, __arg0, __arg1, __arg2)     \
    ({                                                          \
        register u64 a0 asm("a0") = (u64)(__arg0);  \
        register u64 a1 asm("a1") = (u64)(__arg1);  \
        register u64 a2 asm("a2") = (u64)(__arg2);  \
        register u64 a6 asm("a6") = (u64)(__funid); \
        register u64 a7 asm("a7") = (u64)(__extid); \
        asm volatile("ecall"                                    \
                     : "+r"(a0)                                 \
                     : "r"(a1), "r"(a2), "r"(a6), "r"(a7)       \
                     : "memory");                               \
    })

#define MESSAGE_LOAD_MODULE 0x12345678

int send_simple_message()
{
	u32 buf[2];
	buf[0] = MESSAGE_LOAD_MODULE;
	buf[1] = 1; // module ID

	u8 receive_buf[10];

	SBI_ECALL(0x19260817, SBI_EXT_EBI_SEND_MESSAGE, 0, (u64)&buf, 5);
	SBI_ECALL(0x19260817, SBI_EXT_EBI_SUSPEND, 0, 0, 0);

	debug("Back from host\n");

	SBI_ECALL(0x19260817, SBI_EXT_EBI_LISTEN_MESSAGE, 0, (u64)&receive_buf, 10);
	SBI_ECALL(0x19260817, SBI_EXT_EBI_SUSPEND, 0, 0, 0);

	for (int i = 0; i < 10; i++) {
		debug("0x%x\t", receive_buf[i]);
	}
	debug("\n");

	return 0;
}

void emain()
{
	debug("[emain] hello world\n");

	send_simple_message();
}
