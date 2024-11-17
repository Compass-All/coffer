#!/bin/bash
# Output Image File
ROOTFS=tools/rootfs/old-root.img

# Input Sources
MNT=tools/rootfs/mnt
TMP_MNT=tools/tmp_mnt
UBUNTU=ubuntu-rootfs.tar.gz  # ubuntu rootfs
RCS=tools/rootfs/script
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

if test ! -e $RCS
then
    echo "Error: script file not found:" $RCS
    exit 1
fi

if test ! -e $UBUNTU
then
    echo "Error: Ubuntu image file not found:" $UBUNTU
    exit 1
fi

echo "[*] Creating image file:" $ROOTFS
dd if=/dev/zero of=$ROOTFS bs=4M count=512
if [ $? -ne 0 ]; then echo "Error dd!"; exit 1; fi
mkfs.ext4 -F $ROOTFS
if [ $? -ne 0 ]; then echo "Error mkfs!"; exit 1; fi

echo "[*] Mount image..."
mkdir -p $MNT  # mount point

mount -o loop $ROOTFS $MNT
if [ $? -ne 0 ]; then echo "Error mount!"; exit 1; fi

# echo "[*] Copying Ubuntu RootFS"
tar -zxf $UBUNTU -C $MNT
if [ $? -ne 0 ]; then echo "Error tar!"; umount $MNT; exit 1; fi

# Copy init.d script
cp $RCS $MNT/etc/init.d/rcS
chmod +x $MNT/etc/init.d/rcS
if [ $? -ne 0 ]; then echo "Error cp!"; umount $MNT; exit 1; fi

echo "[*] Copying emodules and payloads"
cp -r $PROG $MNT
mkdir -p $MNT/emodules
cp $EMOD/*/*.bin.signed $MNT/emodules
if [ $? -ne 0 ]; then echo "Error cp!"; umount $MNT; exit 1; fi

echo "[*] umount"
umount -l $MNT;
if [ $? -ne 0 ]; then echo "Error umount!"; exit 1; fi
