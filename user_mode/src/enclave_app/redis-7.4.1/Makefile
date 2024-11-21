# Top level makefile, the real shit is at src/Makefile

default: all

.DEFAULT:
	cd src && $(MAKE) -j$(nproc) DEBUG= CC=riscv64-linux-musl-gcc CXX=riscv64-linux-musl-g++ AR=riscv64-linux-musl-ar RANLIB=riscv64-linux-musl-ranlib NM=riscv64-linux-musl-nm $@

install:
	cd src && $(MAKE) $@

.PHONY: install
