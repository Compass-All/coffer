src_dir=$(CURDIR)

ifeq ($(shell uname -s),Darwin)
READLINK ?= greadlink
else
READLINK ?= readlink
endif

src_dir=$(CURDIR)
ifdef O
 build_dir=$(shell $(READLINK) -f $(O))
else
 build_dir=$(CURDIR)/../../build
endif

MODULE=$(shell basename $(CURDIR))

target_dir=$(build_dir)/emodules/$(MODULE)

include_dir=$(CURDIR)/../../include

ifdef CROSS_COMPILE
CC		=	$(CROSS_COMPILE)gcc
CPP		=	$(CROSS_COMPILE)cpp
AR		=	$(CROSS_COMPILE)ar
LD		=	$(CROSS_COMPILE)ld
OBJCOPY		=	$(CROSS_COMPILE)objcopy
else
CC		?=	gcc
CPP		?=	cpp
AR		?=	ar
LD		?=	ld
OBJCOPY		?=	objcopy
endif
AS		=	$(CC)
DTC		=	dtc

# --- FUNCTIONS ---

uniq = $(if $1,$(firstword $1) $(call uniq,$(filter-out $(firstword $1),$1)))

# --- FLAGS ---

include $(CURDIR)/config.mk
include $(CURDIR)/objects.mk

GENFLAGS    = 
GENFLAGS    +=  $(emodule-genflags)

GENFLAGS    +=  -I$(include_dir)

CFLAGS		=	-g -Wall -Werror -ffreestanding -nostdlib -fno-strict-aliasing
CFLAGS		+=	-fno-omit-frame-pointer -fno-optimize-sibling-calls
CFLAGS		+=	-mno-save-restore -mstrict-align
CFLAGS      +=  -static -mcmodel=medany
CFLAGS		+=	$(GENFLAGS)
CFLAGS		+=	-fno-pie -no-pie
ifeq (, $(EMODULES_DEBUG))
CFLAGS		+= -O2
else
CFLAGS		+= -O0
endif

CPPFLAGS	+=	$(GENFLAGS)

ASFLAGS		=	-g -Wall -nostdlib -D__ASSEMBLER__
ASFLAGS		+=	-fno-omit-frame-pointer -fno-optimize-sibling-calls
ASFLAGS		+=	-mno-save-restore -mstrict-align
ASFLAGS		+=	$(GENFLAGS)

ARFLAGS		=	rcs

ELFFLAGS	+=	-Wl,--build-id=none -N -static-libgcc -lgcc

MERGEFLAGS	+=	-r

DTSCPPFLAGS	=	$(CPPFLAGS) -nostdinc -nostdlib -fno-builtin -D__DTS__ -x assembler-with-cpp

# --- TARGETS ---

target = $(target_dir)/$(MODULE).bin
target-objs-y = $(foreach obj,$(emodule-objs-y),$(target_dir)/lib/$(obj))

target_subdirs = $(foreach d,$(filter-out ./,$(dir $(emodule-objs-y))),$(target_dir)/lib/$(d))

all: $(target)

.PHONY: lds

lds: $(target_dir)/$(MODULE).ld

$(target): $(target_dir)/$(MODULE).elf
	$(OBJCOPY) -O binary --set-section-flags .bss=alloc,load,contents $< $@

$(target_dir)/$(MODULE).elf: $(target-objs-y) $(target_dir)/$(MODULE).ld dirs
	$(CC) $(CFLAGS) $(target-objs-y) -T $(target_dir)/$(MODULE).ld -o $@

$(target_dir)/$(MODULE).ld: $(src_dir)/$(MODULE).lds
	$(CPP) $(CPPFLAGS) -x c $< | grep -v "\#" > $@

$(target_dir)/lib/%.o: $(src_dir)/%.c dirs
	$(CC) $(CFLAGS) -c $< -o $@

$(target_dir)/lib/%.o: $(src_dir)/%.S dirs
	$(AS) $(ASFLAGS) -c $< -o $@

.PHONY: dirs
.SILENT: dirs

dirs:
	mkdir -p $(target_dir)
	mkdir -p $(target_dir)/lib
ifneq (, $(target_subdirs))
	mkdir -p $(call uniq,$(target_subdirs))
endif

.PHONY: clean

clean: dirs
	rm -rf $(target_dir)