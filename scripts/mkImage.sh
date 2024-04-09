#!/usr/bin/bash

KERNEL=tools/linux/build/Image
RCS=tools/rootfs/script
PROG=build/prog
EMOD=build/emodules

# Create disk image
dd if=/dev/zero of=disk.img bs=1M count=256
sudo parted disk.img mklabel gpt

# loop set up
pos=`sudo losetup --find --show disk.img`
echo $pos

# Partitions
sudo parted --align minimal $pos mkpart primary ext4 0 25% 
sudo parted --align minimal $pos mkpart primary ext4 25% 100%
sudo parted $pos print

sudo mkfs.ext4 $pos"p1"
sudo mkfs.ext4 $pos"p2"

sudo parted $pos set 1 boot on
# rootfs flag usually set by bootloader
# sudo parted $pos set 2 root on

# Copy kernel and rootfs
sudo rm -rf /mnt/kernel/
sudo rm -rf /mnt/rootfs/
sudo mkdir /mnt/kernel/
sudo mkdir /mnt/rootfs/

sudo mount $pos"p1" /mnt/kernel
sudo mount $pos"p2" /mnt/rootfs

sudo cp $KERNEL /mnt/kernel
sudo cp -r ./busybox/_install/* /mnt/rootfs

# Create a few directories for mounting key filesystems
sudo mkdir -p /mnt/rootfs/proc /mnt/rootfs/sys /mnt/rootfs/dev

# Create a init.d for startup scripts:
sudo mkdir -p /mnt/rootfs/etc/init.d

# BusyBox runs a script /etc/init.d/rcS on system startup.
# NOTE: necessary

# touch ./rcS
# echo "#!/bin/sh" >> ./rcS
# echo 'echo "Hello RISC-V World!"' >> ./rcS
# sudo chmod u+x ./rcS
# sudo mv ./rcS /mnt/rootfs/etc/init.d/rcS

sudo cp $RCS /mnt/rootfs/etc/init.d/rcS
sudo chmod +x /mnt/rootfs/etc/init.d/rcS

echo "[*] Copying emodules and payloads"
sudo cp -r $PROG /mnt/rootfs
sudo mkdir -p /mnt/rootfs/emodules
sudo cp $EMOD/*/*.bin.signed /mnt/rootfs/emodules
if [ $? -ne 0 ]; then echo "Error cp!"; umount $MNT; exit 1; fi


sudo umount /mnt/kernel
sudo umount /mnt/rootfs

# Detach
sudo losetup -d $pos