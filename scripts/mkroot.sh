#!/bin/bash

if [ $# != 1 ]
then
	echo "Error: Invalid input"
	exit 1
fi

ROOTFS_SOURCE=$1
ROOTFS_DIR=tools/rootfs
ROOTFS_IMG=$ROOTFS_DIR/rootfs.img
MNT=$ROOTFS_DIR/mnt

SPL=build/u-boot/u-boot-spl.bin
UBOOT=build/u-boot/u-boot.itb

PROG=build/prog
EMOD=build/emodules

if test ! -d $PROG
then
	echo "Error: payloads not found"
fi

if test ! -d $EMOD
then
	echo "Error: emodules not found"
fi

if test ! -f $ROOTFS_SOURCE
then
	echo "Error: Invalid input"
	exit 1
fi

printf "[*] mkdir\n"
sudo mkdir -p $MNT

printf "[*] creating image file\n"
sudo dd if=/dev/zero of=$ROOTFS_IMG bs=1M count=3072
sudo sgdisk -g --clear --set-alignment=1 \
    --new=1:34:+1M:    --change-name=1:'u-boot-spl'    --typecode=1:5b193300-fc78-40cd-8002-e86c45580b47 \
	--new=2:2082:+4M:  --change-name=2:'opensbi-uboot' --typecode=2:2e54b353-1271-4842-806f-e436d6af6985 \
    --new=3:16384:-0   --change-name=3:'root'          --attributes=3:set:2  \
    $ROOTFS_IMG
if [ $? -ne 0 ]
then
	echo "Error creating image!"
	exit 1
fi

printf "[*] mounting image in loop devices\n"
LOOP=`sudo losetup --partscan --find --show $ROOTFS_IMG`
if [ $? -ne 0 ]
then
	echo "Error mount!"
	exit 1
fi
echo "Loop device:" $LOOP

printf "[*] Writing image\n"
sudo dd if=$SPL of="${LOOP}p1" bs=4k iflag=fullblock oflag=direct conv=fsync status=progress
if [ $? -ne 0 ]
then
	echo "Error dd part1!"
	sudo losetup -d $LOOP
	exit 1
fi
sudo dd if=$UBOOT of="${LOOP}p2" bs=4k iflag=fullblock oflag=direct conv=fsync status=progress
if [ $? -ne 0 ]
then
	echo "Error dd part2!"
	sudo losetup -d $LOOP
	exit 1
fi

# Create and mount root filesystem
printf "[*] mkfs\n"
sudo mkfs.ext4 "${LOOP}p3"
if [ $? -ne 0 ]
then
	echo "Error mkfs!"
	sudo losetup -d $LOOP
	exit 1
fi
sudo e2label "${LOOP}p3" root

printf "[*] mount /root\n"
sudo mount "${LOOP}p3" $MNT
if [ $? -ne 0 ]
then
	echo "Error mount!"
	sudo losetup -d $LOOP
	exit 1
fi

printf "[*] extracting files from source...\n"
sudo tar zxf $ROOTFS_SOURCE --directory=$MNT
printf "[*] copying payloads and emodules\n"
sudo cp -r $EMOD $PROG $MNT
if [ $? -ne 0 ]
then
	echo "Error cp!"
	sudo losetup -d $LOOP
	sudo umount $MNT
	exit 1
fi

printf "[*] umount\n"
sudo umount $MNT
sudo losetup -d $LOOP
