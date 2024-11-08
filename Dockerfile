# 0. Base image (includes RISC-V toolchain)
FROM monstrousmoonshine/riscv_dev
ARG DEBIAN_FRONTEND=noninteractive

# 1. Basic dependencies
RUN apt -y update \
    && apt -y upgrade \
    && apt install -y git vim htop magic-wormhole net-tools nethogs wget csh iputils-ping tree \
    autoconf automake autotools-dev curl python3 libmpc-dev libmpfr-dev libgmp-dev \
    gawk build-essential bison flex texinfo gperf libtool patchutils bc zlib1g-dev libexpat-dev texinfo \
    libncurses5-dev libncursesw5-dev libpython2.7 pkg-config libglib2.0-dev libpixman-1-dev \
    device-tree-compiler swig ninja-build  cmake \
    && apt install -y python3-pip libssl-dev xz-utils zsh
RUN pip3 install setuptools importlib_resources

# zsh + oh-my-zsh
# Uses "p10k" theme with some customization. Uses some bundled plugins and installs some more from github
RUN sh -c "$(wget -O- https://github.com/deluan/zsh-in-docker/releases/download/v1.2.0/zsh-in-docker.sh)" -- \
    # -t https://github.com/romkatv/powerlevel10k.git \
    -p git \
    -p https://github.com/zsh-users/zsh-autosuggestions \
    -p https://github.com/zsh-users/zsh-syntax-highlighting \
    -p https://github.com/agkozak/zsh-z

# 2. Get sources
# busybox, u-boot are moved out.
ARG QEMU_VERSION=5.0.0
ARG LINUX_VERSION=5.14
RUN wget https://download.qemu.org/qemu-${QEMU_VERSION}.tar.xz -P /root --no-verbose
RUN wget https://cdn.kernel.org/pub/linux/kernel/v5.x/linux-${LINUX_VERSION}.tar.xz -P /root --no-verbose   
RUN git clone https://github.com/richfelker/musl-cross-make.git /root/musl-cross-make

# 3. Extract sources
RUN tar -xvf /root/qemu-${QEMU_VERSION}.tar.xz -C /root
RUN tar -xvf /root/linux-5.14.tar.xz -C /root

# 4. Install QEMU
WORKDIR /root/qemu-${QEMU_VERSION}
RUN ./configure --target-list=riscv64-linux-user,riscv64-softmmu \
    && make -j$(nproc) \
    && make install
WORKDIR /

# 5. Install musl-libc
ARG MUSL_VERSION=1.2.3
# 5.1 Patching musl-cross-make
RUN sed -i "s/^MUSL_VER = .*/MUSL_VER = ${MUSL_VERSION}/" /root/musl-cross-make/Makefile
ADD tools/musl/patch /root/musl_patch
RUN mkdir -p /root/musl-cross-make/patches/musl-1.2.3 \
    && cp /root/musl_patch/* /root/musl-cross-make/patches/musl-1.2.3/
# 5.2 Compile musl-libc
WORKDIR /root/musl-cross-make
RUN make TARGET=riscv64-linux-musl install -j$(nproc) 
ENV RISCV_MUSL "/root/musl-cross-make/output"
ENV PATH "$RISCV_MUSL/bin:$PATH"
WORKDIR /

# 6. Patching and configuring Linux
ADD tools/linux/patch /root/linux_patch
ADD tools/linux/config /root/linux_config

WORKDIR /root/linux-${LINUX_VERSION}
RUN git apply /root/linux_patch/mem_hotremove_01.patch \
    && git apply /root/linux_patch/mem_hotremove_02.patch \
    && git apply /root/linux_patch/load_addr.patch \
    && cp /root/linux_config/coffer_defconfig arch/riscv/configs/coffer_defconfig \
    && ARCH=riscv make coffer_defconfig
WORKDIR /

RUN rm -rf *.tar.xz
