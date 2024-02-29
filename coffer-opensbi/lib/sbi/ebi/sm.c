#include <sbi/ebi/sm.h>
#include <sbi/ebi/enclave.h>
#include <sbi/sbi_ecall.h>
#include <sbi/ebi/ipi.h>
#include <sbi/ebi/ebi_debug.h>
#include <sbi/ebi/partition_pool.h>
#include <sbi/ebi/iomngr.h>

int init_sm()
{
	int rc;

	init_enclave_desc();

	// init_memory_pool();

    rc = sbi_ecall_register_extension(&ecall_ebi);
    if (rc) {
        return rc;
    }

    rc = init_partition_pool();
    if (rc) {
        return rc;
    }

    rc = init_ipi();
    if (rc) {
        return rc;
    }

    rc = init_device();
    if (rc) {
        return rc;
    }

    sbi_INFO("enclaves init successfully!\n");
    sbi_INFO("support enclave max num: %d\n", NUM_ENCLAVE);

	return 0;
}