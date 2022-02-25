FROM monstrousmoonshine/riscv_dev
ARG DEBIAN_FRONTEND=noninteractive

# Basic
RUN apt -y update \
    && apt -y upgrade \
    && apt install -y git vim htop magic-wormhole net-tools nethogs wget csh iputils-ping tree
# GNU Environment
RUN apt -y install autoconf automake autotools-dev curl python3 libmpc-dev libmpfr-dev libgmp-dev \
    gawk build-essential bison flex texinfo gperf libtool patchutils bc zlib1g-dev libexpat-dev texinfo \
    libncurses5-dev libncursesw5-dev libpython2.7 pkg-config

# U-Boot
RUN apt -y install u-boot-tools

# Get Sources
ARG QEMU_VERSION=5.0.0
RUN wget https://download.qemu.org/qemu-${QEMU_VERSION}.tar.xz -P /root --no-verbose
RUN git clone https://github.com/torvalds/linux.git /root/linux
RUN git clone https://git.busybox.net/busybox /root/busybox
RUN git clone https://github.com/MstMoonshine/COFFER_prog.git /root/prog

# QEMU
RUN apt -y install libglib2.0-dev libpixman-1-dev \
    && tar xJf /root/qemu-${QEMU_VERSION}.tar.xz -C /root

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
    && cp /root/linux_config/coffer_defconfig arch/riscv/configs/coffer_defconfig \
    && ARCH=riscv make coffer_defconfig
WORKDIR /

# Busybox
ADD tools/busybox/config /root/busybox_config

WORKDIR /root/busybox
RUN git checkout 1_32_1 \
    && cp /root/busybox_config/.config . \
    && CROSS_COMPILE=riscv64-unknown-linux-gnu- make -j4
WORKDIR /