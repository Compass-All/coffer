target_dir := ../../../../build/prog/enclave/$(shell basename $(CURDIR))
include_dir := ../../../include

CC := riscv64-linux-musl-gcc 

CFLAGS := -O2 -static -I$(include_dir) -lm -pthread

src := $(wildcard *.c)
cpp_src := $(wildcard *.cc)

targets := $(src:.c=)

all: dir $(addprefix $(target_dir)/, $(targets))

dir:
	mkdir -p $(target_dir)

$(target_dir)/%: %.c
	$(CC) $< -o $@ $(CFLAGS)

clean:
	-@rm -rf $(target_dir)