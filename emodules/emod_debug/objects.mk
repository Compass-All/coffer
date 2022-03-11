emodule-objs-y = 
emodule-objs-y += emod_debug.o
emodule-objs-y += dependency.o
emodule-objs-y += printf.o

emodule-genflags = -DEMODULES_DEBUG=$(EMODULES_DEBUG)