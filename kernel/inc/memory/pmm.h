// Physical Memory Management

#ifndef __MEMORY_PMM
#define __MEMORY_PMM

#include <types.h>

// This struct must match the 24-byte entry format from the E820 call
typedef struct __attribute__((packed)) {
    uint64_t base;
    uint64_t length;
    uint32_t type;
    uint32_t acpi_extended_attributes;
} e820_entry_t;

void init_pmm();
uint64_t pmm_get_total_memory_mb();
void pmm_print_kernel_info();
void pmm_print_map();

void* pmm_alloc_page();
void pmm_free_page(void* ptr);


#endif