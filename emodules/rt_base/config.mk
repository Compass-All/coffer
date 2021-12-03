# Enclave runtime memory definition
ERT_VA_START=0xC0000000
# ERT_BASE_START=0xE0600000
ERT_BASE_START=$(ERT_VA_START)
ERT_STACK_TOP=0xB0000000
ERT_STACK_SIZE=0x8000
ERT_DRV_START=0xD0000000
ERT_MEM_SIZE=0x200000
ERT_EXTRA_START=0x700000000UL

# Enclace user memory definition
EUSR_STACK_TOP=0x600000000UL
EUSR_STACK_SIZE=0x10000
EUSR_HEAP_START=0x100000000UL