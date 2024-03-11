#!/bin/bash

ROOTFS=tools/rootfs/old-root.img
MNT=tools/rootfs/mnt
BUSYBOX=busybox/_install
RCS=tools/rootfs/script
PROG=build/prog
EMOD=build/emodules

# if test ! -d $PROG
# then
# 	echo "Error: payload directory not found:" $PROG
# 	exit 1
# fi

# if test ! -d $EMOD
# then
# 	echo "Error: emodules directory not found:" $EMOD
# 	exit 1
# fi

echo "[*] Creating image file:" $ROOTFS
dd if=/dev/zero of=$ROOTFS bs=1M count=1024
if [ $? -ne 0 ]; then echo "Error dd!"; exit 1; fi
mkfs.ext4 -F $ROOTFS
if [ $? -ne 0 ]; then echo "Error mkfs!"; exit 1; fi

echo "[*] Mount image..."
sudo mkdir -p $MNT
sudo mount -o loop $ROOTFS $MNT
if [ $? -ne 0 ]; then echo "Error mount!"; exit 1; fi

echo "[*] Copying busybox"
CWD=`pwd`
cd $MNT
sudo mkdir -p bin etc/init.d dev lib sys proc sbin tmp usr \
	usr/bin usr/lib usr/sbin
cd -
sudo cp -r $BUSYBOX/* $MNT
sudo cp $RCS $MNT/etc/init.d/rcS
sudo chmod +x $MNT/etc/init.d/rcS

if [ $? -ne 0 ]; then echo "Error cp!"; umount $MNT; exit 1; fi
sudo ln -s ../bin/busybox $MNT/sbin/init
sudo ln -s ../bin/busybox $MNT/bin/sh
cd $CWD

echo "[*] Copying emodules and payloads"
sudo cp -r $PROG $MNT
sudo mkdir -p $MNT/emodules
sudo cp $EMOD/*/*.bin.signed $MNT/emodules
if [ $? -ne 0 ]; then echo "Error cp!"; umount $MNT; exit 1; fi

echo "[*] umount"
sudo umount -l $MNT;
if [ $? -ne 0 ]; then echo "Error umount!"; exit 1; fi