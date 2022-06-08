# Draft for memory migration

## Key function: region_migration

0. Before migration:
	- register page table root offset relative to pa_start of the primary parition on creating enclave
1. Copy the partition, set owner
2. Transverse the page table and update it (no matter the migrated partition is primary or not)
	- For each pte (both tree pte and leaf pte), check whether pte.ppn is in the range of the migrated region. If so, change the ppn accordingly.
3. Free the source partition