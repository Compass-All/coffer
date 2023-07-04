#!/bin/bash

set -x

#KERNEL=/root/linux/arch/riscv/boot/Image
KERNEL=tools/linux/build/Image
BIOS=build/opensbi/fw_jump.elf
ROOTFS=tools/rootfs/old-root.img

# qemu-system-riscv64 -M virt -m 16G -smp 4 -nographic \
qemu-system-riscv64 -M virt -m 16G -smp 1 -nographic \
    -bios $BIOS \
    -kernel $KERNEL \
    -device loader,file=$KERNEL,addr=0x80200000 \
    -drive file=$ROOTFS,format=raw,id=hd0 \
    -device virtio-blk-device,drive=hd0 \
    -netdev user,id=net0 \
    -device virtio-net-pci,netdev=net0,mac=52:54:00:6f:55:cc,failover=on \
    -append "root=/dev/vda rw console=ttyS0 movablecore=0x240000000"