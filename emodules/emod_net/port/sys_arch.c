#include "arch/sys_arch.h"
#include "timestamp.h"
#include <string.h>

u32 sys_now()
{
	emod_net_timestamp ts;
	emod_net_timestamp_get(&ts);
    return (u32)(ts >> 32);
}

void free(void *ptr)
{
    emod_alloc.emod_alloc_api.free(ptr);
}

void *malloc(size_t size)
{
    return emod_alloc.emod_alloc_api.malloc(size);
}

void *calloc(size_t count, size_t size)
{
    return emod_alloc.emod_alloc_api.calloc(count, size);
}

void *memalign(size_t alignment, size_t size)
{
	return emod_alloc.emod_alloc_api.memalign(alignment, size);
}

int atoi(const char *strn)
{
	int ret = 0;
	int i = 0;
	while (strn[i] != '\0') {
		ret = ret * 10 + strn[i] - '0';
		i++;
	}
	return ret;
}

paddr_t ukplat_virt_to_phys(const void *vaddr)
{
	return get_pa((vaddr_t)vaddr);
}