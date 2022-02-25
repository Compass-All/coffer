emodule-objs-y = 
emodule-objs-y += entry.o 
emodule-objs-y += boot/boot.o
emodule-objs-y += printf/printf.o

emodule-genflags = -DEMODULES_DEBUG=$(EMODULES_DEBUG)