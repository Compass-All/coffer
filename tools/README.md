# Deploy-assistant Tools

This directory contains auxillary scripts for minimal deploy.

## How to use?

### Preliminaries

Specify `MKIMAGE_PATH` for the scripts to locate `mkimage` in U-Boot tools. To make the machine-specific ITB, put ITS and DTB files corresponding to the machine into one directory.

For example, for SiFive HiFive Unmatched, the file layout should be:

```text
unmatched/
├── hifive-unmatched-a00.dtb
└── u-boot.its
```

Note that `u-boot.its` should match the specific format. Use relative path based on the ITS file. See [this example](unmatched/u-boot.its).

### Building the ITB

Run:

```text
MACHINE=<directory-name> MKIMAGE_PATH=<directory-containing-mkimage> tools/doitb
```

A `u-boot.itb` file will be generated at `${PWD}`, or `${ITB_DIR}` (if specified).

### Deploying

Run:

```text
sudo dd if=u-boot.itb of=/dev/sdX bs=2M iflag=fullblock oflag=direct conv=fsync status=progress
```

Note: Replace `sdX` with the specific partition.

## Image Layout

### SiFive HiFive Unmatched

According to the [manual][unmatched_manual], the SD card image consists of four partitions:

1. (RAW format, 1MiB) U-Boot SPL binary
1. (RAW format, 4MiB) U-Boot ITB binary, generated by `doitb`
1. (FAT16 format) `extlinux` configuration, device tree, and kernel image
1. (EXT4 format) Root filesystem

### D1 Nezha

TODO

[unmatched_manual]: https://sifive.cdn.prismic.io/sifive/05d149d5-967c-4ce3-a7b9-292e747e6582_hifive-unmatched-sw-reference-manual-v1p0.pdf