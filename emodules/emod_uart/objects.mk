emodule-objs-y = 
emodule-objs-y += emod_uart.o 
emodule-objs-y += dependency.o 
emodule-objs-y += tmp_printf.o 
emodule-objs-y += serial.o 

ifeq ($(TARGET_PLATFORM), qemu)
emodule-genflags += -D__QEMU__
else ifeq  ($(TARGET_PLATFORM), unmatched)
emodule-genflags += -D__UNMATCHED__
endif