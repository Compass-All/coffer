# Note: this makefile should be run in docker

# Target support: qemu, unmatched (default)
TARGET_PLATFORM ?= qemu
# DEBUG=y to enable debug logging
DEBUG ?= n 

BUILD_DIR = build

SIGN_UTIL_SRC = tools/sign
SIGN_UTIL = $(SIGN_UTIL_SRC)/build/sign

DTS_DIR = tools/dts
DTB_DIR = $(BUILD_DIR)/dtb

LINUX_DIR = tools/linux
LINUX_IMAGE_DIR = $(LINUX_DIR)/build
LINUX_IMAGE = $(LINUX_IMAGE_DIR)/Image
LINUX_SRC = /root/linux

EMOD_MANAGER_SRC = emodules/emod_manager
EMOD_MANAGER_BIN = $(BUILD_DIR)/emodules/emod_manager/emod_manager.bin

OPENSBI_SRC = coffer-opensbi
FW_DIR = $(shell pwd)/$(BUILD_DIR)/opensbi
FW_DYNAMIC_BIN = $(FW_DIR)/fw_dynamic.bin
FW_JUMP_BIN = $(FW_DIR)/fw_jump.bin

UBOOT_DIR = tools/u-boot
UBOOT_IMAGE_DIR = $(BUILD_DIR)/u-boot
UBOOT_SPL_IMAGE = $(UBOOT_IMAGE_DIR)/u-boot-spl.bin
UBOOT_IMAGE = $(UBOOT_IMAGE_DIR)/u-boot.itb
UBOOT_SRC = /root/u-boot

all: sd_part1 sd_part2 prog emodules $(LINUX_IMAGE) opensbi

sd_part1: $(UBOOT_SPL_IMAGE)
sd_part2: $(UBOOT_IMAGE)

prog:
	@printf "\n[.] Building Payloads...\n"
	make -C coffer_user_mode -j$(nproc)
	@printf "[*] Building Payloads Done...\n\n"

$(SIGN_UTIL):
	make -C tools/sign

emodules: $(SIGN_UTIL)
	@printf "\n[.] Building EModules...\n"
	CROSS_COMPILE=riscv64-unknown-elf- \
	TARGET_PLATFORM=$(TARGET_PLATFORM) \
	DEBUG=$(DEBUG) \
	make -C emodules -j$(nproc)
	@printf "[*] Building EModules Done\n\n"

$(DTB_DIR)/%.dtb: $(DTS_DIR)/%.dts
	@printf "\n[.] Building dtb...\n"
	mkdir -p $(DTB_DIR)
	dtc -I dts -O dtb -o $@ $<
	@printf "[*] Building dtb Done...\n\n"

qemu-dtb:
	@printf "\n[.] Dump qemu dtb...\n"
	mkdir -p $(DTB_DIR)
	qemu-system-riscv64 -M virt,dumpdtb=$(DTB_DIR)/qemu.dtb
	@printf "[*] qemu dtb dumped...\n\n"

$(LINUX_IMAGE):
	@printf "\n[.] Building Kernel Image...\n"
	mkdir -p $(LINUX_IMAGE_DIR)
	ARCH=riscv \
	CROSS_COMPILE=riscv64-unknown-linux-gnu- \
	make -C $(LINUX_SRC) -j$$(($$(nproc)-4)) Image
	cp $(LINUX_SRC)/arch/riscv/boot/Image $@
	@printf "[*] Building Kernel Image Done...\n\n"

$(EMOD_MANAGER_BIN):
	@printf "\n[.] Building EMod_Manager...\n"
	CROSS_COMPILE=riscv64-unknown-elf- \
	TARGET_PLATFORM=$(TARGET_PLATFORM) \
	DEBUG=$(DEBUG) \
	make -C $(EMOD_MANAGER_SRC) -j$(nproc)
	@printf "[*] Building EMod_Manager Done...\n\n"

opensbi:
	@printf "\n[.] Building Security Monitor (dynamic)...\n"
	mkdir -p $(FW_DIR)
	CROSS_COMPILE=riscv64-unknown-linux-gnu- \
	PLATFORM=generic \
	TARGET_PLATFORM=$(TARGET_PLATFORM) \
	DEBUG=$(DEBUG) \
	make -C $(OPENSBI_SRC) -j$(nproc)
	cp $(OPENSBI_SRC)/build/platform/generic/firmware/fw_dynamic.bin $(FW_DYNAMIC_BIN)
	@printf "[*] Building Security Monitor Done (dynamic)...\n\n"

$(FW_DYNAMIC_BIN): opensbi
$(FW_JUMP_BIN): opensbi

$(UBOOT_IMAGE): $(FW_DYNAMIC_BIN) $(DTB_DIR)/hifive-unmatched-a00.dtb
	@printf "\n[.] Building U-Boot Image...\n"
	mkdir -p $(UBOOT_IMAGE_DIR)
	CROSS_COMPILE=riscv64-unknown-linux-gnu- \
	OPENSBI=$(FW_DYNAMIC_BIN) \
	make -C $(UBOOT_SRC) -j$(nproc)
	cp $(UBOOT_SRC)/u-boot.itb $@
	@printf "[*] Building U-Boot Image Done...\n\n"

$(UBOOT_SPL_IMAGE): $(FW_DYNAMIC_BIN) $(DTB_DIR)/hifive-unmatched-a00.dtb
	@printf "\n[.] Building U-Boot SPL Image...\n"
	mkdir -p $(UBOOT_IMAGE_DIR)
	CROSS_COMPILE=riscv64-unknown-linux-gnu- \
	OPENSBI=$(FW_DYNAMIC_BIN) \
	make -C $(UBOOT_SRC) -j$(nproc)
	cp $(UBOOT_SRC)/spl/u-boot-spl.bin $@
	@printf "[*] Building U-Boot SPL Image Done...\n\n"

clean:
	rm -rf $(BUILD_DIR)

clean_all: clean
	make clean -C coffer-opensbi
	make clean -C coffer_user_mode
	make clean -C $(UBOOT_SRC)

.PHONY: all clean clean_all prog emodules opensbi