emodule-objs-y = 
emodule-objs-y += entry.o 
emodule-objs-y += emod_manager.o
emodule-objs-y += boot/boot.o
emodule-objs-y += memory/memory.o
emodule-objs-y += memory/page_pool.o
emodule-objs-y += memory/page_table.o
emodule-objs-y += panic/panic.o
emodule-objs-y += emod_table/emod_table.o
emodule-objs-y += printf/printf.o
emodule-objs-y += debug/debug.o

emodule-genflags = -DEMODULES_DEBUG=$(EMODULES_DEBUG)
emodule-genflags += -DEMOD_MANAGER_SVA_START=$(EMOD_MANAGER_SVA_START)