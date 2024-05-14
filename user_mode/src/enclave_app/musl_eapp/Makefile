target_dir := ../../../../build/prog/enclave/$(shell basename $(CURDIR))
include_dir := ../../../include

MUSL_CC = /home/gartonchan/workspace/coffer_updated/musl-cross-make/output/bin
CC = $(MUSL_CC)/riscv64-linux-musl-gcc
# CC := riscv64-unknown-linux-musl-gcc

CFLAGS := -O2 -static -I$(include_dir) -lm -pthread

src := $(wildcard *.c)
cpp_src := $(wildcard *.cc)

targets := $(src:.c=)

all: dir $(addprefix $(target_dir)/, $(targets))
#all:
#	echo "skip musl_eapp"

dir:
	mkdir -p $(target_dir)

$(target_dir)/%: %.c
	$(CC) $< -o $@ $(CFLAGS)

clean:
	-@rm -rf $(target_dir)