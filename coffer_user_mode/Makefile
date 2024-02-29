# ---------------
# Common settings
BUILD_DIR = ../build/prog
INCLUDE_DIR = ../include
CFLAGS = -Wall -O2 -g -static -I$(INCLUDE_DIR)

# ---------------
# Host settings
HOST_BUILD_DIR = $(BUILD_DIR)/host
HOST_INCLUDE_DIR = include/host_app

HOST_SRC_DIR = src/host_app
HOST_SRC_LIST = $(notdir $(wildcard $(HOST_SRC_DIR)/*.c))
HOST_TARGET = $(addprefix $(HOST_BUILD_DIR)/, $(HOST_SRC_LIST:.c=))

HOST_UTIL_DIR = $(HOST_SRC_DIR)/util
HOST_UTIL_SRC = host_util.c
HOST_UTIL_OBJ = $(HOST_BUILD_DIR)/$(HOST_UTIL_SRC:.c=.o)

HOST_CC = riscv64-unknown-linux-gnu-gcc
HOST_CFLAG = $(CFLAGS)
HOST_CFLAG += -I$(HOST_INCLUDE_DIR)

# ---------------
# Enclave settings
ENCLAVE_BUILD_DIR = $(BUILD_DIR)/enclave
ENCLAVE_INCLUDE_DIR = include/enclave_app

ENCLAVE_SRC_DIR = src/enclave_app
ENCLAVE_SRC_LIST = $(notdir $(wildcard $(ENCLAVE_SRC_DIR)/*.c))
ENCLAVE_TARGET = $(addprefix $(ENCLAVE_BUILD_DIR)/, $(ENCLAVE_SRC_LIST:.c=))

ENCLAVE_UTIL_DIR = $(ENCLAVE_SRC_DIR)/util
ENCLAVE_UTIL_SRC = enclave_util.c
ENCLAVE_UTIL_OBJ = $(ENCLAVE_BUILD_DIR)/$(ENCLAVE_UTIL_SRC:.c=.o)

ENCLAVE_SRC_SUBDIR = $(filter-out $(ENCLAVE_UTIL_DIR)/, $(sort $(dir $(wildcard $(ENCLAVE_SRC_DIR)/*/*))))

ENCLAVE_CC = riscv64-unknown-elf-gcc
# ENCLAVE_CC = riscv64-linux-musl-gcc 
# ENCLAVE_CC = riscv64-unknown-linux-gnu-gcc
ENCLAVE_CFLAG = $(CFLAGS)
ENCLAVE_CFLAG += -I$(ENCLAVE_INCLUDE_DIR)

# ---------------
all: host_apps enclave_apps

clean:
	rm -rf $(BUILD_DIR)
	make clean -C $(ENCLAVE_SRC_DIR)/darknet
	make clean -C $(ENCLAVE_SRC_DIR)/sqlite-bench
	make clean -C $(ENCLAVE_SRC_DIR)/stress-ng

# ---------------
host_dir:
	mkdir -p $(HOST_BUILD_DIR)

host_apps: host_dir $(HOST_UTIL_OBJ) $(HOST_TARGET)

$(HOST_UTIL_OBJ): $(HOST_UTIL_DIR)/$(HOST_UTIL_SRC) \
		$(HOST_INCLUDE_DIR)/$(HOST_UTIL_SRC:.c=.h)
	$(HOST_CC) -c $< -o $@ $(HOST_CFLAG)

$(HOST_BUILD_DIR)/% : $(HOST_SRC_DIR)/%.c $(HOST_UTIL_OBJ)
	$(HOST_CC) -c $< -o $@.o $(HOST_CFLAG)
	$(HOST_CC) $(HOST_UTIL_OBJ) $@.o -o $@ $(HOST_CFLAG)
	rm $@.o

# ---------------
enclave_dir:
	mkdir -p $(ENCLAVE_BUILD_DIR)

enclave_apps: enclave_dir $(ENCLAVE_UTIL_OBJ) $(ENCLAVE_TARGET) subdirs

$(ENCLAVE_UTIL_OBJ): $(ENCLAVE_UTIL_DIR)/$(ENCLAVE_UTIL_SRC) \
		$(ENCLAVE_INCLUDE_DIR)/$(ENCLAVE_UTIL_SRC:.c=.h)
	$(ENCLAVE_CC) -c $< -o $@ $(ENCLAVE_CFLAG)

$(ENCLAVE_BUILD_DIR)/% : $(ENCLAVE_SRC_DIR)/%.c $(ENCLAVE_UTIL_OBJ)
	$(ENCLAVE_CC) -c $< -o $@.o $(ENCLAVE_CFLAG)
	$(ENCLAVE_CC) $(ENCLAVE_UTIL_OBJ) $@.o -o $@ $(ENCLAVE_CFLAG)
	rm $@.o

subdirs:
	for subdir in $(ENCLAVE_SRC_SUBDIR) ; do \
        make -C $$subdir -j; \
    done
