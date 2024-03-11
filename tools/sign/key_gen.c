#include <stdio.h>
#include <stdlib.h>
#include "ecc.h"

int main()
{
	uint8_t *pub_key;
	uint8_t *pri_key;
	int ret;

	pub_key = (uint8_t *)malloc(ECC_BYTES + 1);
	pri_key = (uint8_t *)malloc(ECC_BYTES);

	ret = ecc_make_key(pub_key, pri_key);
	printf("ret = %d\n", ret);

	printf("public key:\n");
	for (int i = 0; i < ECC_BYTES + 1; i++) {
		printf("0x%x,", pub_key[i]);
	}
	printf("\n");

	printf("private key:\n");
	for (int i = 0; i < ECC_BYTES; i++) {
		printf("0x%x,", pri_key[i]);
	}
	printf("\n");
	
	free(pub_key);
	free(pri_key);
}