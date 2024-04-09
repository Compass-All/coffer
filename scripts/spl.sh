#!/bin/bash

set -x

#KERNEL=/root/linux/arch/riscv/boot/Image
KERNEL=tools/linux/build/Image
UBOOT_SPL=build/u-boot/u-boot-spl.bin
UBOOT_FIT=build/u-boot/u-boot.itb

ROOTFS=tools/rootfs/old-root.img

# qemu-system-riscv64 -M virt -m 16G -smp 4 -nographic \
# qemu-system-riscv64 -M virt -m 16G -smp 1 -nographic \

qemu-system-riscv64 -M virt -m 16G -smp 4 -nographic \
    -bios $UBOOT_SPL \
    -kernel $KERNEL \
    -device loader,file=$UBOOT_FIT,addr=0x80300000 \
    -blockdev driver=file,filename=./disk.img,node-name=disk \
    -device virtio-blk-device,drive=disk \

    # -drive file=$ROOTFS,format=raw,id=hd0 \
    # -device virtio-blk-device,drive=hd0 \
    # -netdev user,id=net0,hostfwd=tcp::8080-:80 \
    # -device virtio-net-pci,netdev=net0,mac=52:54:00:6f:55:cc,failover=on \
    # -object filter-dump,id=f1,netdev=net0,file=net0.pcap \
    # -append "root=/dev/vda rw console=ttyS0 movablecore=0x240000000"
    #  --trace "virtio_*"
    # -netdev tap,id=net0,ifname=tap0,script=no,downscript=no \

# KERNEL=tools/linux/build/Image
# BIOS=build/u-boot/u-boot-spl.bin
# ROOTFS=tools/rootfs/old-root.img
# qemu-system-riscv64 -machine virt -smp 2 -m 2G -nographic \
#     -bios $BIOS \
#     -device loader,file=./build/u-boot/u-boot.itb,addr=0x80200000 \
#     -blockdev driver=file,filename=$ROOTFS,node-name=disk \
#     -device virtio-blk-device,drive=disk \
