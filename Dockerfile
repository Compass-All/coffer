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

# QEMU
RUN apt -y install libglib2.0-dev libpixman-1-dev

RUN wget https://download.qemu.org/qemu-5.0.0.tar.xz -P /root --no-verbose
RUN tar xJf /root/qemu-5.0.0.tar.xz -C /root

WORKDIR /root/qemu-5.0.0
RUN ./configure --target-list=riscv64-linux-user,riscv64-softmmu
RUN make -j$(nproc)
RUN make install
WORKDIR /

# U-Boot
RUN apt -y install u-boot-tools