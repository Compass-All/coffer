#!/bin/bash

if [ $# != 3 ]
then
	echo "Error: Invalid input"
	exit 1
fi

KERNEL=$1
TARGET=$2
DTB=$3
ROOTFS_SOURCE=$1
ROOTFS_DIR=tools/rootfs
ROOTFS_IMG=$ROOTFS_DIR/rootfs.img
MNT=$ROOTFS_DIR/mnt

if test ! -f $KERNEL
then
	echo "Error Kernel not found"
fi

if test ! -f $ROOTFS_IMG
then
	echo "ROOTFS Image not found"
fi

if test ! -f $DTB
then
	echo "DTB not found"
fi

printf "[*] mounting image in loop devices\n"
LOOP=`sudo losetup --partscan --find --show $ROOTFS_IMG`
if [ $? -ne 0 ]
then
	echo "Error mount!"
	exit 1
fi
echo "Loop device:" $LOOP

printf "[*] mount /root\n"
sudo mount "${LOOP}p3" $MNT
if [ $? -ne 0 ]
then
	echo "Error mount!"
	sudo losetup -d $LOOP
	exit 1
fi

printf "[*] cp kernel image\n"
sudo cp $KERNEL $MNT/boot/$TARGET
if [ $? -ne 0 ]
then
	echo "Error cp kernel!"
	sudo losetup -d $LOOP
	sudo umount $MNT
	exit 1
fi

printf "[*] cp dtb\n"
sudo cp $DTB $MNT/boot/dtbs
if [ $? -ne 0 ]
then
	echo "Error cp dtb!"
	sudo losetup -d $LOOP
	sudo umount $MNT
	exit 1
fi

printf "[*] umount\n"
sudo umount $MNT
sudo losetup -d $LOOP