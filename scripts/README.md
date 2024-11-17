### scripts
#### boot with `fw_jump` (for fast testing)
**mkroot.sh**: generating a basic `busybox` rootfs.
**mkroot-ubuntu.sh**: generating a `ubuntu` rootfs.
**run-qemu.sh**: launch coffer on qemu by directly passing the parameters.

#### boot with `fw_dynamic` (fot emulating the boot flow on the real board)
**mkImage.sh**: generating a image with two partitions (for kernel and rootfs (busybox) respectively).
**run-spl.sh**: uboot-spl -> (opensbi) -> uboot-fit -> kernel -> rootfs
