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

## Enclave lifecycle


### Create

param:				()
return to enclave:	(eid, s mode payload pa start, s mode payload size)

### Enter

param:				(eid, argc, argv,
						u mode payload host va start, u mode payload size)
return to enclave:	(umode payload pa start, u mode payload size, argc)

### Suspend

param:			(short message)
return to host:	(short message)

### Resume

param:				(eid, short message)
return to enclave:	(short message)

### Exit

param:			()
return to host:	(return value)