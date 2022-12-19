#!/bin/bash

TARGET=$1
SOURCE=build/u-boot/u-boot-spl.bin

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

if test ! -f $SOURCE
then
	echo "Error: file not found:" $SOURCE
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

printf "\n[*] Burning...\n\n"
echo $SOURCE "->" $TARGET
sudo dd if=$SOURCE of=$TARGET bs=2M conv=fsync status=progress
printf "\n[*] Done!\n"