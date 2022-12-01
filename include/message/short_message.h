#pragma once

#define EXIT_ENCLAVE	0x0UL
#define LOAD_MODULE		(0x11223344UL << 32)
#define GET_MODULE_SIZE	(0xCAFEUL << 32)
#define TIMER_INTERRUPT	(0x22334455UL << 32)
#define GET_FILE		(0x33445566UL << 32)
#define GET_FILE_SIZE	(0x44556677UL << 32)

#define TASK_MASK	(0xFFFFFFFFUL << 32)