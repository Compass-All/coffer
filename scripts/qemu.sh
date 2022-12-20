#!/bin/bash

SPL=/root/u-boot/spl/u-boot-spl.bin
UBOOT=/root/u-boot/u-boot.itb
ROOTFS=tools/rootfs/rootfs.img

qemu-system-riscv64 -M virt -smp 4 -m 16G \
    -display none -serial stdio \
    -bios $SPL \
    -device loader,file=$UBOOT,addr=0x80400000 \
	-drive file=$ROOTFS,format=raw,id=hd0 \
    -device virtio-blk-device,drive=hd0

    # -device ich9-ahci,id=ahci \
    # -drive if=none,file=$ROOTFS,format=raw,id=mydisk \
    # -device ide-hd,drive=mydisk,bus=ahci.0

