FROM monstrousmoonshine/riscv_dev
ARG DEBIAN_FRONTEND=noninteractive

# Basic
RUN apt -y update \
    && apt -y upgrade \
    && apt install -y git vim htop magic-wormhole net-tools nethogs wget csh iputils-ping tree \
    autoconf automake autotools-dev curl python3 libmpc-dev libmpfr-dev libgmp-dev \
    gawk build-essential bison flex texinfo gperf libtool patchutils bc zlib1g-dev libexpat-dev texinfo \
    libncurses5-dev libncursesw5-dev libpython2.7 pkg-config libglib2.0-dev libpixman-1-dev \
    device-tree-compiler swig libssl-dev

# Get Sources
ARG QEMU_VERSION=5.0.0
RUN wget https://download.qemu.org/qemu-${QEMU_VERSION}.tar.xz -P /root --no-verbose
RUN git clone https://github.com/torvalds/linux.git /root/linux
RUN git clone https://git.busybox.net/busybox /root/busybox
RUN git clone https://github.com/richfelker/musl-cross-make.git /root/musl-cross-make
RUN git clone https://github.com/u-boot/u-boot.git /root/u-boot
RUN tar xJf /root/qemu-${QEMU_VERSION}.tar.xz -C /root

# MUSL
ADD tools/musl/patch /root/musl_patch
RUN mkdir -p /root/musl-cross-make/patches/musl-1.2.3 \
    && cp /root/musl_patch/0001-memset_main_tls.diff /root/musl-cross-make/patches/musl-1.2.3/0001-memset_main_tls.diff \
    && cp /root/musl_patch/0002-assert.diff /root/musl-cross-make/patches/musl-1.2.3/0002-assert.diff

WORKDIR /root/musl-cross-make
RUN make TARGET=riscv64-linux-musl install -j$(nproc) 
ENV RISCV_MUSL "/root/musl-cross-make/output"
ENV PATH "$RISCV_MUSL/bin:$PATH"
WORKDIR /

# QEMU
WORKDIR /root/qemu-${QEMU_VERSION}
RUN ./configure --target-list=riscv64-linux-user,riscv64-softmmu \
    && make -j$(nproc) \
    && make install
WORKDIR /

# Linux
ADD tools/linux/patch /root/linux_patch
ADD tools/linux/config /root/linux_config

WORKDIR /root/linux
RUN git checkout v5.14 \
    && git apply /root/linux_patch/mem_hotremove_01.patch \
    && git apply /root/linux_patch/mem_hotremove_02.patch \
    && git apply /root/linux_patch/load_addr.patch \
    && cp /root/linux_config/coffer_defconfig arch/riscv/configs/coffer_defconfig \
    && ARCH=riscv make coffer_defconfig
WORKDIR /

# Busybox
ADD tools/busybox/config /root/busybox_config

WORKDIR /root/busybox
RUN git checkout 1_32_1 \
    && cp /root/busybox_config/.config . \
    && CROSS_COMPILE=riscv64-unknown-linux-gnu- make -j
WORKDIR /

# u-boot
ADD tools/u-boot/patch /root/u-boot_patch
ADD tools/u-boot/config /root/u-boot_config

WORKDIR /root/u-boot
RUN git checkout d637294e264adfeb29f390dfc393106fd4d41b17 \
    && git apply /root/u-boot_patch/unmatched.patch \
    && cp /root/u-boot_config/coffer_defconfig /root/u-boot/configs \
    && CROSS_COMPILE=riscv64-unknown-linux-gnu- make coffer_defconfig
WORKDIR /
