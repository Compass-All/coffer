#!/bin/bash

TARGET=$1
PROG=build/prog
EMOD=build/emodules

if [ $# != 1 ]
then
	echo "Error: Invalid input"
	exit 1
fi

if test ! -b $1
then
	echo "Error: Unrecognised block device"
	exit 1
fi

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

printf "\n[*] Disk information:\n\n"
sudo fdisk -l $TARGET
printf "\n"

read -p "Are you sure? " -n 1 -r
printf "\n"
if [[ ! $REPLY =~ ^[Yy]$ ]]
then
    [[ "$0" = "$BASH_SOURCE" ]] && exit 1 || return 1 # handle exits from shell or function but don't exit interactive shell
fi

MNT=/run/media/root
printf "[*] Mounting ...\n"
sudo mkdir -p $MNT
if [ $? -ne 0 ]; then echo "Error mkdir!"; exit 1; fi
sudo mount $TARGET $MNT
if [ $? -ne 0 ]; then echo "Error mount!"; exit 1; fi
printf "[*] Copying ...\n"
sudo cp -r $EMOD $PROG $MNT
if [ $? -ne 0 ]; then echo "Error cp!"; sudo umount $MNT; exit 1; fi
printf "[*] Unmounting ...\n"
sudo umount $MNT
if [ $? -ne 0 ]; then echo "Error umount!"; exit 1; fi
printf "[*] Done!\n"