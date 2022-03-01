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

int send_simple_message()
{
	char buf[5];
	buf[0] = 0x11;
	buf[1] = 0x22;
	buf[2] = 0x33;
	buf[3] = 0x44;
	buf[4] = 0x55;

	SBI_ECALL(0x19260817, SBI_EXT_EBI_SEND_MESSAGE, 0, (u64)&buf, 5);
	SBI_ECALL(0x19260817, SBI_EXT_EBI_SUSPEND, 0, 0, 0);

	return 0;
}

void emain()
{
	debug("[emain] hello world\n");

	send_simple_message();
}
