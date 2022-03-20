# Enclave

An enclave consists of the following information:
	- A unique EID
	- Status
	- Context
		- User mode context
		- CSR context
	- Memory regions
		- Components
			- Memory partitions -> managed by SM memory manager
				- Active
			- MMIO regions (peripheral regions)
				- Active: set in PMP registers
				- Inactive: have permission but haven't been set (waiting for scheduling)
		- Can be checked by "software PMP" (only take active regions into consideration)
			- Known usages of software PMP:
				- message_channel
					- argc, argv
					- emodules loading
		- IO Scheduler:
			- Manage active/inactive status of MMIO regions according to certain scheduling algorithms


S mode tries to read/write: 

S mode page table
	- constant offset:
		- 2M megapage
		- For data access (at C level)
		- No need to consider migration
		- Motivation: access pa
	- contiguous va:
		- 4k page
		- For execution (possibly including data access at assembly level)
		- Need to support migration