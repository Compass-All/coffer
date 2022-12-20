#!/bin/bash

ROOTFS=/root/coffer/tools/rootfs/old-root.img
MNT=/root/coffer/tools/rootfs/mnt
BUSYBOX=/root/busybox/busybox
RCS=/root/coffer/tools/rootfs/script
PROG=build/prog
EMOD=build/emodules

if test ! -d $PROG
then
	echo "Error: payload directory not found:" $PROG
	exit 1
fi

if test ! -d $EMOD
then
	echo "Error: emodules directory not found:" $EMOD
	exit 1
fi

echo "[*] Creating image file:" $ROOTFS
dd if=/dev/zero of=$ROOTFS bs=1M count=1024
if [ $? -ne 0 ]; then echo "Error dd!"; exit 1; fi
mkfs.ext4 -F $ROOTFS
if [ $? -ne 0 ]; then echo "Error mkfs!"; exit 1; fi

echo "[*] Mount image..."
mkdir -p $MNT
mount -o loop $ROOTFS $MNT
if [ $? -ne 0 ]; then echo "Error mount!"; exit 1; fi

echo "[*] Copying busybox"
CWD=`pwd`
cd $MNT
mkdir -p bin etc/init.d dev lib sys proc sbin tmp usr \
	usr/bin usr/lib usr/sbin
cp $BUSYBOX $MNT/bin
cp $RCS $MNT/etc/init.d/rcS
chmod +x $MNT/etc/init.d/rcS

if [ $? -ne 0 ]; then echo "Error cp!"; umount $MNT; exit 1; fi
ln -s ../bin/busybox $MNT/sbin/init
ln -s ../bin/busybox $MNT/bin/sh
cd $CWD

echo "[*] Copying emodules and payloads"
cp -r $PROG $MNT
cp $EMOD/*/*.bin.signed $MNT
if [ $? -ne 0 ]; then echo "Error cp!"; umount $MNT; exit 1; fi

echo "[*] umount"
umount -l $MNT;
if [ $? -ne 0 ]; then echo "Error umount!"; exit 1; fi