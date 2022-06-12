emodule-objs-y = 
emodule-objs-y += emod_debug.o
emodule-objs-y += printf.o
emodule-objs-y += timer.o
emodule-objs-y += dependency.o

emodule-genflags =

ifeq ($(DEBUG), y)
emodule-genflags += -D__DEBUG__
endif
