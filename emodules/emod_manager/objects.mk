emodule-objs-y = 
emodule-objs-y += entry.o 
emodule-objs-y += emod_manager.o
emodule-objs-y += boot/boot.o
emodule-objs-y += memory/memory.o
emodule-objs-y += memory/page_pool.o
emodule-objs-y += memory/page_table.o
emodule-objs-y += elf/elf_loader.o
emodule-objs-y += panic/panic.o
emodule-objs-y += trap/exceptions.o
emodule-objs-y += trap/syscalls.o
emodule-objs-y += emod_table/emod_table.o
emodule-objs-y += printf/printf.o
emodule-objs-y += debug/debug.o

# emodule-genflags = -DEMODULES_DEBUG=$(EMODULES_DEBUG)
emodule-genflags += -DEMOD_MANAGER_VA_START=$(EMOD_MANAGER_VA_START)
emodule-genflags += -DEMOD_INIT_BRK=$(EMOD_INIT_BRK)
emodule-genflags += -DEMOD_MANAGER_LINEAR_START=$(EMOD_MANAGER_LINEAR_START)
emodule-genflags += -DEMOD_MANAGER_LINEAR_SIZE=$(EMOD_MANAGER_LINEAR_SIZE)
emodule-genflags += -DEMOD_MANAGER_LINEAR_OFFSET=$(EMOD_MANAGER_LINEAR_OFFSET)