emodule-objs-y = 
emodule-objs-y += entry.o 
emodule-objs-y += emod_manager.o
emodule-objs-y += boot/boot.o
emodule-objs-y += attest/ecc.o
emodule-objs-y += attest/md2.o
emodule-objs-y += attest/key.o
emodule-objs-y += memory/memory.o
emodule-objs-y += memory/page_pool.o
emodule-objs-y += memory/page_table.o
emodule-objs-y += elf/elf_loader.o
emodule-objs-y += panic/panic.o
emodule-objs-y += trap/exceptions.o
emodule-objs-y += trap/syscalls.o
emodule-objs-y += trap/interrupts.o
emodule-objs-y += emod_table/emod_table.o
emodule-objs-y += printf/printf.o
emodule-objs-y += util/string.o
emodule-objs-y += eval/eval.o
emodule-objs-y += debug/debug.o

emodule-genflags = 
emodule-genflags += -DEMOD_MANAGER_VA_START=$(EMOD_MANAGER_VA_START)
emodule-genflags += -DEMOD_INIT_BRK=$(EMOD_INIT_BRK)
emodule-genflags += -DEMOD_MANAGER_LINEAR_START=$(EMOD_MANAGER_LINEAR_START)
emodule-genflags += -DEMOD_MANAGER_LINEAR_SIZE=$(EMOD_MANAGER_LINEAR_SIZE)
emodule-genflags += -DEMOD_MANAGER_LINEAR_OFFSET=$(EMOD_MANAGER_LINEAR_OFFSET)
emodule-genflags += -DEUSR_HEAP_START_ALIGNED=$(EUSR_HEAP_START_ALIGNED)
emodule-genflags += -DUMODE_STACK_TOP_VA=$(UMODE_STACK_TOP_VA)
emodule-genflags += -DUMODE_STACK_SIZE=$(UMODE_STACK_SIZE)
emodule-genflags += -DSMODE_STACK_SIZE=$(SMODE_STACK_SIZE)
emodule-genflags += -DATTEST_PUB_KEY=$(ATTEST_PUB_KEY)
# emodule-genflags += -DATTEST_PRI_KEY=$(ATTEST_PRI_KEY)


ifeq ($(TARGET_PLATFORM), qemu)
emodule-genflags += -D__QEMU__
else ifeq  ($(TARGET_PLATFORM), unmatched)
emodule-genflags += -D__UNMATCHED__
endif

ifeq ($(DEBUG), y)
emodule-genflags += -D__DEBUG__
endif