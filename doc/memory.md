# Memory Management of Emodule manager

Emodule manager provides basic memory management functionalities. For advanced functionalities, an additional memory allocation emodule is required.

The basic functionalities include page mapping, one simple memory pool (which only allocates but never deallocates) for either of the emod_manager and the U mode ELF bss sections. If ELF bss sections are too larged to be put into this memory pool, the memory allocation emodule shall be loaded.

## Memory layout

To support memory migration, memory layout of the enclave must be carefully considered.

### PA Space

An enclave consists of a primary memory partition and multiple secondary memory partitions, one for each of the loaded emodules. Emodule manager resides in the primary partition.

### VA Space


```
S mode VA Space:

0x8000_0000 ~ 0x0x80_8000_0000: 512GB
	Enclave Primary Start ~ Enclave Primary End: 2MB (Partition size)
```