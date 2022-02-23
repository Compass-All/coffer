MKIMAGE ?= /home/prongs/src/u-boot/tools/mkimage
ITS_PATH ?= tools/unmatched
ITB_PATH ?= .

all: emodules opensbi image

image: tools/md2/build/md2 emodules opensbi
	$(MKIMAGE) -E -f $(ITS_PATH)/u-boot.its $(ITB_PATH)/u-boot.itb

tools/md2/build/md2:
	make -C tools/md2

emodules: clean
	make -C emodules CROSS_COMPILE=riscv64-unknown-elf- -j

opensbi: clean
	make -C coffer-opensbi CROSS_COMPILE=riscv64-unknown-elf- PLATFORM=generic -j

clean:
	make clean -C emodules
	make clean -C coffer-opensbi

.PHONY: all clean