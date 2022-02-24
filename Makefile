MKIMAGE ?= mkimage
ITS_PATH ?= tools/unmatched
ITB_PATH ?= build/itb

all: emodules opensbi image

image: emodules opensbi
	mkdir -p $(ITB_PATH)
	$(MKIMAGE) -E -f $(ITS_PATH)/u-boot.its $(ITB_PATH)/u-boot.itb

emodules: tools/md2/build/md2
	make -C emodules CROSS_COMPILE=riscv64-unknown-elf- -j

opensbi:
	make -C coffer-opensbi CROSS_COMPILE=riscv64-unknown-elf- PLATFORM=generic -j

tools/md2/build/md2:
	make -C tools/md2

clean:
	make clean -C emodules
	make clean -C coffer-opensbi
	rm -rf $(ITB_PATH)

.PHONY: all clean