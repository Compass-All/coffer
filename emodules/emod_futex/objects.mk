emodule-objs-y = 
emodule-objs-y += emod_futex.o 
emodule-objs-y += dependency.o 
emodule-objs-y += atomic.o 
emodule-objs-y += futex.o 
emodule-objs-y += lock.o 

emodule-genflags = 
ifeq ($(TARGET_PLATFORM), qemu)
emodule-genflags += -D__QEMU__
else ifeq  ($(TARGET_PLATFORM), unmatched)
emodule-genflags += -D__UNMATCHED__
endif