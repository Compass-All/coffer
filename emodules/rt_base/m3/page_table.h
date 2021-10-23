#pragma once

// PTE fields
#define PTE_V 0x001 // Valid
#define PTE_R 0x002 // Read
#define PTE_W 0x004 // Write
#define PTE_X 0x008 // Execute
#define PTE_U 0x010 // User
#define PTE_G 0x020 // Global
#define PTE_A 0x040 // Accessed
#define PTE_D 0x080 // Dirty
#define PTE_SOFT 0x300 // Reserved for software