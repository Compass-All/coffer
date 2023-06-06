emodule-genflags = 
emodule-genflags += -I$(CURDIR)/port/include
emodule-genflags += -I$(CURDIR)/port/include/arch
emodule-genflags += -I$(CURDIR)/lwip/src/include
emodule-genflags += -I$(CURDIR)/lwip/src/include/ipv4

ifeq ($(TARGET_PLATFORM), qemu)
emodule-genflags += -D__QEMU__
else ifeq  ($(TARGET_PLATFORM), unmatched)
emodule-genflags += -D__UNMATCHED__
endif

ifeq ($(DEBUG), y)
emodule-genflags += -D__DEBUG__
endif