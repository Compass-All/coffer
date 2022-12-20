#!/bin/bash

set -x

KERNEL=/root/linux/arch/riscv/boot/Image
BIOS=build/opensbi/fw_jump.elf
ROOTFS=tools/rootfs/old-root.img

qemu-system-riscv64 -M virt -m 16G -smp 4 -nographic \
    -bios $BIOS \
    -kernel $KERNEL \
    -device loader,file=$KERNEL,addr=0x80200000 \
    -drive file=$ROOTFS,format=raw,id=hd0 \
    -device virtio-blk-device,drive=hd0 \
    -append "root=/dev/vda rw console=ttyS0 movablecore=0x240000000"