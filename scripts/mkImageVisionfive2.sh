#!/bin/bash
OUTPUT=vf2-sdcard.img

UBOOT_SPL_SRC=build/u-boot/u-boot-spl.bin
UBOOT_SPL=build/u-boot/u-boot-spl.bin.normal.out
UBOOT_FIT=build/u-boot/u-boot.itb
KERNEL=tools/linux/build/Image

if test ! -f $UBOOT_SPL_SRC
then
	echo "Error: file not found:" $UBOOT_SPL_SRC
	exit 1
fi

./tools/visionfive2Tools/spl_tool/spl_tool -c -f $UBOOT_SPL_SRC

if test ! -f $UBOOT_SPL
then
	echo "Error: file not found:" $UBOOT_SPL
	exit 1
fi

if test ! -f $UBOOT_FIT
then
	echo "Error: file not found:" $UBOOT_FIT
	exit 1
fi

sudo dd if=/dev/zero of=$OUTPUT bs=4M count=768

# [ uboot-spl | uboot-fit | rootfs (including kernel) ]
sudo sgdisk -g --clear --set-alignment=1 \
    --new=1:4096:8191 --change-name=1:spl --typecode=1:2E54B353-1271-4842-806F-E436D6AF6985  \
	--new=2:8192:16383 --change-name=2:uboot --typecode=2:BC13C2FF-59E6-4262-A352-B275FD6F7172  \
 	--new=3:16384:-0 --change-name=3:boot --typecode=3:EBD0A0A2-B9E5-4433-87C0-68B6B72699C7  \
	$OUTPUT

sudo partprobe $OUTPUT
LOOP_DEV=`sudo losetup --partscan --find --show $OUTPUT`
echo $LOOP_DEV

sudo partprobe $LOOP_DEV
# p1
sudo dd if=./build/u-boot/u-boot-spl.bin.normal.out of=${LOOP_DEV}p1 bs=4k iflag=fullblock oflag=direct conv=fsync status=progress
# p2
sudo dd if=./build/u-boot/u-boot.itb of=${LOOP_DEV}p2 bs=4k iflag=fullblock oflag=direct conv=fsync status=progress
# p3
sudo mkfs.ext4 ${LOOP_DEV}p3
sudo mount ${LOOP_DEV}p3 /mnt
sudo tar -zxf ubuntu-rootfs.tar.gz -C /mnt

# Opt1. make boot directory and copy kernel
# bootcmd: ext4load mmc 1:3 84000000 Image; booti 0x84000000
# bootargs: root=... rootwait rw console=ttySIF0,115200n8
sudo cp $KERNEL /mnt/

# Opt2. make extlinux.conf to boot kernel
if test ! -d /mnt/boot/dtbs/5.11.12/starfive
then
	sudo mkdir -p /mnt/boot/dtbs/5.11.12/starfive
fi
sudo cp u-boot/arch/riscv/dts/jh7110-starfive-visionfive-2.dtb /mnt/boot/dtbs/5.11.12/starfive/

echo  \
"menu title StarFive VisionFive2 Boot Options
timeout 200
default kernel-5.11.12

label kernel-5.11.12
        menu label Linux kernel-5.11.12
        kernel /boot/vmlinuz-5.11.12
        fdt /boot/dtbs/5.11.12/starfive/jh7110-starfive-visionfive-2.dtb
        initrd /boot/initrd.img-5.11.12
        append earlyprintk rw root=/dev/mmcblk1p3 rootfstype=ext4 rootwait console=ttyS0,115200 LANG=en_US.UTF-8 earlycon movablecore=0x240000000

label recovery-kernel-5.11.12
        menu label Linux kernel-5.11.12 (recovery mode)
        kernel /boot/vmlinuz-5.11.12
        fdt /boot/dtbs/5.11.12/starfive/jh7110-starfive-visionfive-2.dtb
        initrd /boot/initrd.img-5.11.12
        append earlyprintk rw root=/dev/mmcblk1p3 rootfstype=ext4 rootwait console=ttyS0,115200 LANG=en_US.UTF-8 earlycon single" \
> extlinux.conf

sudo cp extlinux.conf /mnt/boot/extlinux/extlinux.conf
rm extlinux.conf

sudo umount ${LOOP_DEV}p3
sudo losetup -d $LOOP_DEV

# sudo dd if=vf-sdcard.img of=/dev/sdX bs=4M conv=fsync status=progress
# sudo screen /dev/ttyUSB0 115200 -

# echo "- +" | sudo sfdisk -N 3 /dev/sdx
# sudo resize2fs /dev/sdx3
