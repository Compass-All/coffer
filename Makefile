# todo:
# differnt targets: make qemu and make unmatched
TARGET_PLATFORM ?= qemu # 1. qemu 2. unmatched
DEBUG ?= n

BUILD_DIR = build

MKIMAGE ?= mkimage
ITS_PATH ?= tools/unmatched
ITB_PATH ?= $(BUILD_DIR)/itb

SIGN_PATH ?= tools/sign

BOARD_ROOTFS_PARTITION ?= /dev/mmcblk0p4
BOARD_ROOTFS_PATH ?= /run/media/mingde/root

DOCKER = sudo docker # Linux only
DOCKER_WORKDIR = /root/coffer
DOCKER_RUN = $(DOCKER) run -it --rm \
	-v $(shell pwd):$(DOCKER_WORKDIR) \
	--device /dev/kvm coffer_dev \
	env TERM=xterm-256color
DOCKER_GDB = $(DOCKER) run -it --rm \
	-v $(shell pwd):$(DOCKER_WORKDIR) \
	--device /dev/kvm coffer_dev \
	env TERM=xterm-256color
DOCKER_MAKE = $(DOCKER_RUN) make
DOCKER_IMAGE = coffer_dev

DOCKER_BUSYBOX_PATH = /root/busybox
DOCKER_LINUX_PATH = /root/linux
DOCKER_PROG_PATH = /root/prog

ROOTFS = $(BUILD_DIR)/rootfs.img
MOUNT_POINT = $(BUILD_DIR)/mnt

EMODULE_DIRS = $(subst emodules/,,$(sort $(dir $(wildcard emodules/*/))))
EMODULES = $(EMODULE_DIRS:/=)
EMODULE_TARGETS = $(addsuffix .bin.signed, $(EMODULES))
EMODULE_PATH = $(addprefix $(BUILD_DIR)/emodules/, $(EMODULE_DIRS))
EMODULE_TARGETS_ABS = $(join $(EMODULE_PATH), $(EMODULE_TARGETS))

KERNEL_IMAGE_PATH = tools/linux/build
KERNEL_IMAGE = $(KERNEL_IMAGE_PATH)/Image

BOARD_DEST ?= /dev/mmcblk0p2

QEMU = qemu-system-riscv64
QEMU_CORES ?= 4
QEMU_INIT_SCRIPT = tools/rootfs/script
QEMU_CMD = -M virt -m 16G -smp $(QEMU_CORES) -nographic \
        -bios $(DOCKER_WORKDIR)/coffer-opensbi/build/platform/generic/firmware/fw_jump.elf \
        -kernel $(DOCKER_WORKDIR)/$(KERNEL_IMAGE) \
        -device loader,file=$(DOCKER_WORKDIR)/$(KERNEL_IMAGE),addr=0x80200000 \
        -drive file=$(DOCKER_WORKDIR)/$(ROOTFS),format=raw,id=hd0 \
        -device virtio-blk-device,drive=hd0 \
        -append "root=/dev/vda rw console=ttyS0 movablecore=0x240000000"
QEMU_LOG_FILE = ~/tmp/qemu-$(shell date +%Y%m%dT%H%M%S).log

PROG_BUILD = $(BUILD_DIR)/prog

all: dir emodules opensbi rootfs

kernel-image: docker
ifeq (, $(wildcard $(KERNEL_IMAGE))) # kernel image not found
	mkdir -p $(KERNEL_IMAGE_PATH)
	$(DOCKER_RUN) /bin/bash \
		-c " make -C $(DOCKER_LINUX_PATH) ARCH=riscv \
		CROSS_COMPILE=riscv64-unknown-linux-gnu- -j$$(($$(nproc)-4)) Image \
		&& cp $(DOCKER_LINUX_PATH)/arch/riscv/boot/Image \
		$(DOCKER_WORKDIR)/$(KERNEL_IMAGE) "
endif

qemu-run: kernel-image docker # rootfs
	$(DOCKER_RUN) $(QEMU) $(QEMU_CMD)

qemu-gdb: kernel-image docker # rootfs
	$(DOCKER_GDB) $(QEMU) $(QEMU_CMD) -s -S

qemu-log: kernel-image docker # rootfs
	$(DOCKER_RUN) $(QEMU) $(QEMU_CMD) | tee $(QEMU_LOG_FILE)

dir:
	mkdir -p $(BUILD_DIR)

docker:
ifeq (, $(shell $(DOCKER) images $(DOCKER_IMAGE) -q))
	$(DOCKER) build -t $(DOCKER_IMAGE) .
endif

prog: docker
ifeq (, $(wildcard $(PROG_BUILD)/))
	$(DOCKER_MAKE) -C $(DOCKER_WORKDIR)/coffer_user_mode
endif

rootfs: emodules docker prog
ifeq (, $(wildcard $(ROOTFS))) # ROOTFS not found
# Make ext2 FS
	sudo dd if=/dev/zero of=$(ROOTFS) bs=1M count=512
	sudo mkfs.ext2 -F $(ROOTFS)
# Mount
	sudo mkdir -p $(MOUNT_POINT)
	sudo mount -o loop $(ROOTFS) $(MOUNT_POINT)
# Setup busybox
	cd $(MOUNT_POINT) && sudo mkdir -p bin etc dev lib sys proc sbin tmp usr usr/bin usr/lib usr/sbin
	$(DOCKER_RUN) cp $(DOCKER_BUSYBOX_PATH)/busybox $(DOCKER_WORKDIR)/$(MOUNT_POINT)/bin
	sudo ln -s ../bin/busybox $(MOUNT_POINT)/sbin/init
	sudo ln -s ../bin/busybox $(MOUNT_POINT)/bin/sh

	sudo umount $(MOUNT_POINT)
endif
# ROOTFS exists
	sudo mount -o loop $(ROOTFS) $(MOUNT_POINT)
# Update shell init script
	sudo mkdir -p $(MOUNT_POINT)/etc/init.d
	sudo cp $(QEMU_INIT_SCRIPT) $(MOUNT_POINT)/etc/init.d/rcS
	sudo chmod +x $(MOUNT_POINT)/etc/init.d/rcS
# Copy emodules
	sudo mkdir -p $(MOUNT_POINT)/emodules
	sudo cp $(EMODULE_TARGETS_ABS) $(MOUNT_POINT)/emodules/
# Copy prog
	sudo rm -rf $(MOUNT_POINT)/prog
	sudo cp -r $(PROG_BUILD) $(MOUNT_POINT)/

	sudo umount $(MOUNT_POINT)


board-image: emodules opensbi docker
	sudo mkdir -p $(ITB_PATH)
	$(DOCKER_RUN) $(MKIMAGE) -E -f $(DOCKER_WORKDIR)/$(ITS_PATH)/u-boot.its $(DOCKER_WORKDIR)/$(ITB_PATH)/u-boot.itb

burn-image:	board-image
	@if test -b $(BOARD_DEST) ; \
	then \
		printf "\nBurning image\n" ;	\
		sudo dd if=$(ITB_PATH)/u-boot.itb of=$(BOARD_DEST) bs=2M iflag=fullblock oflag=direct conv=fsync status=progress ; \
	else \
		printf "\nSD card not inserted\n\n" ; \
	fi;

# -@sudo mount -t ext4 $(BOARD_ROOTFS_PARTITION) $(BOARD_ROOTFS_PATH)
	@if test -d $(BOARD_ROOTFS_PATH) ; \
	then \
		printf "\nupdating prog and emodules\n\n" ; \
		sudo rm -rf $(BOARD_ROOTFS_PATH)/prog $(BOARD_ROOTFS_PATH)/emodules ; \
		sudo mkdir $(BOARD_ROOTFS_PATH)/emodules ; \
		sudo cp -r build/prog $(BOARD_ROOTFS_PATH)/prog ; \
		sudo cp build/emodules/*/*.bin.signed $(BOARD_ROOTFS_PATH)/emodules ; \
		sudo umount $(BOARD_ROOTFS_PATH) ; \
	else \
		printf "\nrootfs not mounted\n\n" ; \
	fi;

# TODO: copy emodules and prog to rootfs on SD card
# copy-file: emodules prog

tools:
	$(DOCKER_MAKE) -C $(DOCKER_WORKDIR)/$(SIGN_PATH)

# do not add "-j" to this target, which leads to UB
emodules: docker tools
	$(DOCKER_MAKE) -C $(DOCKER_WORKDIR)/emodules CROSS_COMPILE=riscv64-unknown-elf- TARGET_PLATFORM=$(TARGET_PLATFORM) DEBUG=$(DEBUG)

opensbi: docker emodules
	$(DOCKER_MAKE) clean -C $(DOCKER_WORKDIR)/coffer-opensbi 
	$(DOCKER_MAKE) -C $(DOCKER_WORKDIR)/coffer-opensbi CROSS_COMPILE=riscv64-unknown-elf- PLATFORM=generic TARGET_PLATFORM=$(TARGET_PLATFORM) DEBUG=$(DEBUG) -j

clean: docker
	sudo rm -rf $(BUILD_DIR)
	sudo make clean -C coffer-opensbi
	sudo make clean -C coffer_user_mode

clean-kernel:
	rm $(KERNEL_IMAGE)

.PHONY: all clean clean-kernel docker rootfs dir board-image kernel-image prog tools
