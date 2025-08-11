// Virtual Memory Management 
#ifndef __MEMORY_VMM
#define __MEMORY_VMM

#include <types.h>

// Page Table Entry (PTE) flags
// permission and status bits for each page
#define PTE_PRESENT (1 << 0)    // Page is present in memory
#define PTE_READ_WRITE (1 << 1) // Page is read/write (0 is read-only)
#define PTE_USER_SUPER (1 << 2) // Page is accessible by user-mode (0 is kernel only)

// Single 64-bit Page Table Entry
typedef uint64_t pte_t;

// 512 Entries per page table (4KB page / 8 bytes entry)
typedef pte_t page_table_t[512];

// Map a single virtual page to physical page
void vmm_map_page(void* virtual_address, void* physical_address);

// Initialize kernel address space
void init_vmm();

// Load PML4 into the CR3 register to activate paging
void vmm_activate();

#endif
