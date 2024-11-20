CWD=$(pwd)

if [ ! -d $CWD/u-boot ]; then
    wget https://source.denx.de/u-boot/u-boot/-/archive/v2024.04/u-boot-v2024.04.tar.gz
    tar -xvf u-boot-v2024.04.tar.gz
    mv u-boot-v2024.04 u-boot
    rm u-boot-v2024.04.tar.gz
    # git clone https://source.denx.de/u-boot/u-boot.git 
    # cd u-boot
    # git checkout v2024.01  # patch for VisionFive2
else
    echo "u-boot directories exist."
fi

if [ ! -d $CWD/busybox ]; then
    # git clone git@github.com:mirror/busybox.git
    wget https://www.busybox.net/downloads/busybox-1.32.1.tar.bz2
    tar -xvf busybox-1.32.1.tar.bz2
    mv busybox-1.32.1 busybox
    rm busybox-1.32.1.tar.bz2
else
    echo "busybox directories exist."
fi

if [ ! -d $CWD/tools/busybox/_install ]; then
    cd $CWD/busybox
    # git checkout 1_32_stable
    cp $CWD/tools/busybox/config/.config $CWD/busybox/
    make CROSS_COMPILE=riscv64-unknown-linux-gnu- -j 
    make CROSS_COMPILE=riscv64-unknown-linux-gnu- install
else 
    echo "busybox compiled."
fi
