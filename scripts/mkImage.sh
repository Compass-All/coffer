#!/bin/bash

IMG=tools/old-root.img
KERNEL=tools/linux/build/Image
RCS=tools/rootfs/script
PROG=build/prog
EMOD=build/emodules

# Mount Point
KERNEL_MNT=/mnt/kernel
ROOTFS_MNT=/mnt/rootfs

# Create disk image
dd if=/dev/zero of=$IMG bs=4M count=512
parted $IMG mklabel gpt

# loop set up
pos=`losetup --find --show $IMG`
echo $pos
if [ $? -ne 0 ]; then echo "Error losetup!"; exit 1; fi

# Partitions
parted --align minimal $pos mkpart primary ext4 0 15% 
if [ $? -ne 0 ]; then echo "Error parted!"; exit 1; fi
parted --align minimal $pos mkpart primary ext4 15% 100%
if [ $? -ne 0 ]; then echo "Error parted!"; exit 1; fi
parted $pos print
if [ $? -ne 0 ]; then echo "Error parted!"; exit 1; fi

# Format as ext4
mkfs.ext4 $pos"p1"
if [ $? -ne 0 ]; then echo "Error mkfs!"; exit 1; fi
mkfs.ext4 $pos"p2"
if [ $? -ne 0 ]; then echo "Error mkfs!"; exit 1; fi

# Set boot flag
parted $pos set 1 boot on
# rootfs flag usually set by bootloader
# parted $pos set 2 root on

# Check if the dependencies exists
if test ! -f $KERNEL
then
    echo "Error: file not found:" $KERNEL
    exit 1
fi

if test ! -f $RCS
then
    echo "Error: file not found:" $RCS
    exit 1
fi

if test ! -d $PROG
then
    echo "Error: file not found:" $PROG
    exit 1
fi

if test ! -d $EMOD
then
    echo "Error: file not found:" $EMOD
    exit 1
fi

# Check if the mount points exist and mount
if test ! -d $KERNEL_MNT
then
    mkdir -p $KERNEL_MNT
fi

if test ! -d $ROOTFS_MNT
then
    mkdir -p $ROOTFS_MNT
fi

mount $pos"p1" $KERNEL_MNT
if [ $? -ne 0 ]; then echo "Error mount!"; umount $KERNEL_MNT; exit 1; fi
mount $pos"p2" $ROOTFS_MNT
if [ $? -ne 0 ]; then echo "Error mount!"; umount $KERNEL_MNT; umount $ROOTFS_MNT; exit 1; fi

# Copy kernel and rootfs
cp $KERNEL $KERNEL_MNT
if [ $? -ne 0 ]; then echo "Error cp!"; umount $KERNEL_MNT; umount $ROOTFS_MNT; exit 1; fi
tar -zxf ubuntu-rootfs.tar.gz -C $ROOTFS_MNT
if [ $? -ne 0 ]; then echo "Error tar!"; umount $KERNEL_MNT; umount $ROOTFS_MNT; exit 1; fi

# Create root directory
mkdir -p /mnt/rootfs/root
if [ $? -ne 0 ]; then echo "Error mkdir!"; umount $KERNEL_MNT; umount $ROOTFS_MNT; exit 1; fi

# Create a few directories for mounting key filesystems
mkdir -p /mnt/rootfs/proc /mnt/rootfs/sys /mnt/rootfs/dev
if [ $? -ne 0 ]; then echo "Error mkdir!"; umount $KERNEL_MNT; umount $ROOTFS_MNT; exit 1; fi

# Create a init.d for startup scripts:
mkdir -p /mnt/rootfs/etc/init.d
if [ $? -ne 0 ]; then echo "Error mkdir!"; umount $KERNEL_MNT; umount $ROOTFS_MNT; exit 1; fi

# BusyBox runs a script /etc/init.d/rcS on system startup.
# NOTE: necessary

cp $RCS /mnt/rootfs/etc/init.d/rcS
if [ $? -ne 0 ]; then echo "Error cp!"; umount $KERNEL_MNT; umount $ROOTFS_MNT; exit 1; fi
chmod +x /mnt/rootfs/etc/init.d/rcS
if [ $? -ne 0 ]; then echo "Error chmod!"; umount $KERNEL_MNT; umount $ROOTFS_MNT; exit 1; fi

echo "[*] Copying emodules and payloads"
cp -r $PROG /mnt/rootfs
if [ $? -ne 0 ]; then echo "Error cp!"; umount $KERNEL_MNT; umount $ROOTFS_MNT; exit 1; fi

mkdir -p /mnt/rootfs/emodules
if [ $? -ne 0 ]; then echo "Error mkdir!"; umount $KERNEL_MNT; umount $ROOTFS_MNT; exit 1; fi

cp $EMOD/*/*.bin.signed /mnt/rootfs/emodules
if [ $? -ne 0 ]; then echo "Error cp!"; umount $KERNEL_MNT; umount $ROOTFS_MNT; exit 1; fi

# temporarily use
cp $KERNEL /mnt/rootfs/root
if [ $? -ne 0 ]; then echo "Error cp!"; umount $KERNEL_MNT; umount $ROOTFS_MNT; exit 1; fi
if [ $? -ne 0 ]; then echo "Error cp!"; umount $MNT; exit 1; fi

umount $KERNEL_MNT
umount $ROOTFS_MNT

# Detach
losetup -d $pos
