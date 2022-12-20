#!/bin/bash

qemu-system-riscv64 -M virt,dumpdtb=dump.dtb

dtc -I dtb -O dts -o qemu.dts dump.dtb