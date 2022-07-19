#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

#include "ecc.h"
#include "md2.h"

#define debug
// #define debug printf

const uint8_t ATTEST_PUB_KEY[] = { 0x2,0x49,0xbe,0x59,0xad,0x6c,0x8a,0xaa,0x8,0x61,0xf8,0x33,0x5e,0xf6,0xcf,0xe8,0xb6,0x8,0x88,0x2c,0xbe,0x3,0x2a,0xed,0x0,0xad,0x4c,0xf6,0x9,0xd5,0xe0,0x7c,0xc8 };

const uint8_t ATTEST_PRI_KEY[] = { 0x82,0xcb,0xce,0x8a,0xbe,0x4f,0x9d,0x9,0xbe,0xb8,0xa4,0x5a,0xcf,0xf8,0x86,0x46,0x80,0x3a,0x9b,0xec,0x22,0x47,0x74,0x2a,0xb8,0xd,0xf1,0x51,0x10,0x34,0xa0,0x5e };

static uint64_t read_file(const char *path, uint8_t *content[])
{
	int read_ret = 1;
	int fd;
	uint64_t len;
	uint64_t offset;

	if (!path) {
		printf("NULL\n");
		exit(0);
	}
	fd = open(path, O_RDONLY);
	if (fd == -1) {
		printf("File %s open failed\n", path);
		perror("Error: ");
		exit(-1);
	}

	len = lseek(fd, 0L, SEEK_END);
	lseek(fd, 0L, SEEK_SET);
	*content = (uint8_t *)malloc(len);

	offset = 0;
	while (read_ret) {
		read_ret = read(fd, *content + offset, 0x400);
		if (read_ret == -1) {
			debug("File %s read failed at offset 0x%lx\n",
				path, offset);
			perror("Error: ");
			close(fd);
			free(*content);
			exit(-1);
		}
		offset += read_ret;
	}
	close(fd);

	debug("File %s len: 0x%lx\n", path, offset);

	return offset;
}

static void compute_md2_hash(
	uint8_t *payload,
	size_t bin_size,
	uint8_t *hash_buffer
)
{
	uint8_t md2_hash[MD2_BLOCK_SIZE];
	md2(payload, bin_size, md2_hash);
	for (int i = 0; i < MD2_BLOCK_SIZE; i++) {
		hash_buffer[i] = md2_hash[i];	
		hash_buffer[i + MD2_BLOCK_SIZE] = md2_hash[i];	
	}

	debug("md2: ");
	for (int i = 0; i < MD2_BLOCK_SIZE * 2; i++)
		debug("0x%x,", hash_buffer[i]);
	debug("\n");
}

static void get_sign(const uint8_t *hash, uint8_t *sign)
{
	ecdsa_sign(ATTEST_PRI_KEY, hash, sign);

	debug("sign: ");
	for (int i = 0; i < ECC_BYTES * 2; i++)
		printf("%02x ", sign[i]);
	printf("\n");
}

int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("Usage: %s [file_to_sign]\n", argv[0]);
		exit(0);
	}

	uint8_t *payload_path = argv[1];
	uint8_t *payload;
	size_t size = read_file(payload_path, &payload);

	uint8_t md2_hash[MD2_BLOCK_SIZE];
	uint8_t hash[MD2_BLOCK_SIZE * 2];
	uint8_t *sign = (uint8_t *)malloc(ECC_BYTES * 2);

	compute_md2_hash(payload, size, hash);
	get_sign(hash, sign);

	int ret = ecdsa_verify(ATTEST_PUB_KEY, hash, sign);
	if (!ret)
		printf("Not valid\n");

	free(payload);
	free(sign);
}