target_dir := ../../../../build/prog/enclave/$(shell basename $(CURDIR))
include_dir := ../../../include

CC := riscv64-unknown-elf-gcc
XX := riscv64-unknown-elf-g++

CFLAGS := -O2 -static -I$(include_dir) -lm

src := $(wildcard *.c)
cpp_src := $(wildcard *.cc)

targets := $(src:.c=)
targets += $(cpp_src:.cc=)

all: dir $(addprefix $(target_dir)/, $(targets))

dir:
	mkdir -p $(target_dir)

$(target_dir)/%: %.c
	$(CC) $< -o $@ $(CFLAGS)

$(target_dir)/%: %.cc
	$(XX) $< -o $@ $(CFLAGS)

clean:
	-@rm -rf $(target_dir)