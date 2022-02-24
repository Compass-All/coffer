BUILD_DIR = build

MKIMAGE ?= mkimage
ITS_PATH ?= tools/unmatched
ITB_PATH ?= $(BUILD_DIR)/itb

DOCKER = sudo docker # Linux only
DOCKER_WORKDIR = /root/coffer
DOCKER_RUN = $(DOCKER) run -it -v $(shell pwd):$(DOCKER_WORKDIR) --rm --device /dev/kvm coffer_dev
DOCKER_MAKE = $(DOCKER_RUN) make
DOCKER_IMAGE = coffer_dev

DOCKER_BUSYBOX_PATH = /root/busybox
DOCKER_LINUX_PATH = /root/linux

ROOTFS = $(BUILD_DIR)/rootfs.img
MOUNT_POINT = $(BUILD_DIR)/mnt

EMODULE_DIRS = $(subst emodules/,,$(sort $(dir $(wildcard emodules/drv_*/))))
EMODULES = $(EMODULE_DIRS:/=)
EMODULE_TARGETS = $(addsuffix .bin, $(EMODULES))
EMODULE_PATH = $(addprefix $(BUILD_DIR)/emodules/, $(EMODULE_DIRS))
EMODULE_TARGETS_ABS = $(join $(EMODULE_PATH), $(EMODULE_TARGETS))

KERNEL_IMAGE_PATH = tools/linux/build
KERNEL_IMAGE = $(KERNEL_IMAGE_PATH)/Image

QEMU_INIT_SCRIPT = tools/rootfs/script

all: dir emodules opensbi board-image rootfs

kernel-image: docker
ifeq (, $(wildcard $(KERNEL_IMAGE))) # kernel image not found
	mkdir -p $(KERNEL_IMAGE_PATH)
	$(DOCKER_RUN) /bin/bash -c " make -C $(DOCKER_LINUX_PATH) ARCH=riscv CROSS_COMPILE=riscv64-unknown-linux-gnu- -j4 Image \
	&& cp $(DOCKER_LINUX_PATH)/arch/riscv/boot/Image $(DOCKER_WORKDIR)/$(KERNEL_IMAGE) "
endif

qemu-run: kernel-image docker
	$(DOCKER_RUN) qemu-system-riscv64 \
		-M virt -m 8G -smp 1 -nographic \
        -bios $(DOCKER_WORKDIR)/coffer-opensbi/build/platform/generic/firmware/fw_jump.elf \
        -kernel $(DOCKER_WORKDIR)/$(KERNEL_IMAGE) \
        -device loader,file=$(DOCKER_WORKDIR)/$(KERNEL_IMAGE),addr=0x80200000 \
        -drive file=$(DOCKER_WORKDIR)/$(ROOTFS),format=raw,id=hd0 \
        -device virtio-blk-device,drive=hd0 \
        -append "root=/dev/vda rw console=ttyS0 movablecore=0x140000000" \

dir:
	mkdir -p $(BUILD_DIR)

docker:
ifeq (, $(shell $(DOCKER) images $(DOCKER_IMAGE) -q))
	$(DOCKER) build -t $(DOCKER_IMAGE) .
endif

rootfs: emodules docker
ifeq (, $(wildcard $(ROOTFS))) # ROOTFS not found
# Make ext2 FS
	sudo dd if=/dev/zero of=$(ROOTFS) bs=1M count=64
	sudo mkfs.ext2 -F $(ROOTFS)
# Mount
	mkdir -p $(MOUNT_POINT)
	sudo mount -o loop $(ROOTFS) $(MOUNT_POINT)
# Setup busybox
	cd $(MOUNT_POINT) && sudo mkdir -p bin etc dev lib sys proc sbin tmp usr usr/bin usr/lib usr/sbin
	$(DOCKER_RUN) cp $(DOCKER_BUSYBOX_PATH)/busybox $(DOCKER_WORKDIR)/$(MOUNT_POINT)/bin
	sudo ln -s ../bin/busybox $(MOUNT_POINT)/sbin/init
	sudo ln -s ../bin/busybox $(MOUNT_POINT)/bin/sh
# Shell init script
	sudo mkdir -p $(MOUNT_POINT)/etc/init.d
	sudo cp $(QEMU_INIT_SCRIPT) $(MOUNT_POINT)/etc/init.d/rcS
	sudo chmod +x $(MOUNT_POINT)/etc/init.d/rcS

	sudo umount $(MOUNT_POINT)
else # ROOTFS exists
	sudo mount -o loop $(ROOTFS) $(MOUNT_POINT)
# Update shell init script
	sudo mkdir -p $(MOUNT_POINT)/etc/init.d
	sudo cp $(QEMU_INIT_SCRIPT) $(MOUNT_POINT)/etc/init.d/rcS
	sudo chmod +x $(MOUNT_POINT)/etc/init.d/rcS
# Copy emodules
	sudo cp $(EMODULE_TARGETS_ABS) $(MOUNT_POINT)/

	sudo umount $(MOUNT_POINT)
endif


board-image: emodules opensbi docker
	mkdir -p $(ITB_PATH)
	$(DOCKER_RUN) $(MKIMAGE) -E -f $(DOCKER_WORKDIR)/$(ITS_PATH)/u-boot.its $(DOCKER_WORKDIR)/$(ITB_PATH)/u-boot.itb

# do not add "-j" to this target, which leads to UB
emodules: tools/md2/build/md2 docker
	$(DOCKER_MAKE) -C $(DOCKER_WORKDIR)/emodules CROSS_COMPILE=riscv64-unknown-elf-

opensbi: docker
	$(DOCKER_MAKE) -C $(DOCKER_WORKDIR)/coffer-opensbi CROSS_COMPILE=riscv64-unknown-elf- PLATFORM=generic -j

tools/md2/build/md2:
	make -C tools/md2

clean: docker
	sudo rm -rf $(BUILD_DIR)
	make clean -C coffer-opensbi

clean-kernel:
	rm $(KERNEL_IMAGE)

.PHONY: all clean clean-kernel docker rootfs dir board-image kernel-image