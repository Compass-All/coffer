# Memory Management of Emodule manager

### Layout

---
Primary Partition
- Emod_manager
- S mode page pool
---
ELF Partition
- ELF sections
- U mode page pool
---
Other S/U partitions
---

### Questions
- Support ELF larger than 2M?

###

There should be a reserved memory region for emod_memory().