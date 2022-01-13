emodule-objs-y =
emodule-objs-y += rt_base.o
emodule-objs-y += rt_console.o
emodule-objs-y += rt_entry.o
emodule-objs-y += rt_syscall.o
emodule-objs-y += rt_trap.o
emodule-objs-y += rt_mod_mngr.o
emodule-objs-y += base_util/elf.o
emodule-objs-y += base_util/md2.o
emodule-objs-y += base_util/memory.o
emodule-objs-y += m3/inv_map.o
emodule-objs-y += m3/page_pool.o
emodule-objs-y += m3/page_table.o

emodule-genflags =
emodule-genflags += -DERT_VA_START=$(ERT_VA_START)
emodule-genflags += -DERT_BASE_START=$(ERT_BASE_START)
emodule-genflags += -DERT_STACK_TOP=$(ERT_STACK_TOP)
emodule-genflags += -DERT_STACK_SIZE=$(ERT_STACK_SIZE)
emodule-genflags += -DERT_DRV_START=$(ERT_DRV_START)
emodule-genflags += -DERT_MEM_SIZE=$(ERT_MEM_SIZE)
emodule-genflags += -DEUSR_STACK_TOP=$(EUSR_STACK_TOP)
emodule-genflags += -DEUSR_STACK_SIZE=$(EUSR_STACK_SIZE)
emodule-genflags += -DEUSR_HEAP_START=$(EUSR_HEAP_START)
emodule-genflags += -DERT_EXTRA_START=$(ERT_EXTRA_START)
emodule-genflags += -DERT_PERI_REG_START=$(ERT_PERI_REG_START)