#!/bin/bash
sudo dd if=/dev/zero of=sdcard.img bs=1M count=3072

sudo sgdisk -g --clear --set-alignment=1 \
    --new=1:4096:8191 --change-name=1:spl --typecode=1:2E54B353-1271-4842-806F-E436D6AF6985  \
	--new=2:8192:16383 --change-name=2:uboot --typecode=2:BC13C2FF-59E6-4262-A352-B275FD6F7172  \
 	--new=3:16384:-0 --change-name=3:boot --typecode=3:EBD0A0A2-B9E5-4433-87C0-68B6B72699C7  \
	sdcard.img

sudo partprobe sdcard.img
LOOP_DEV=`sudo losetup --partscan --find --show sdcard.img`
echo $LOOP_DEV

sudo partprobe $LOOP_DEV
# echo `lsblk $LOOP_DEV`
sudo dd if=./build/u-boot/u-boot-spl.bin.normal.out of=${LOOP_DEV}p1 bs=4k iflag=fullblock oflag=direct conv=fsync status=progress

sudo dd if=./build/u-boot/u-boot.itb of=${LOOP_DEV}p2 bs=4k iflag=fullblock oflag=direct conv=fsync status=progress

sudo mkfs.ext4 ${LOOP_DEV}p3
# sudo parted $LOOP_DEV set 1 boot on
sudo mount ${LOOP_DEV}p3 /mnt
sudo cp -r ./mnt/ubuntu/* /mnt
sudo umount ${LOOP_DEV}p3

sudo losetup -d $LOOP_DEV

