// Physical Memory Management

#ifndef __MEMORY_PMM_H
#define __MEMORY_PMM_H

#include <types.h>

// This struct must match the 24-byte entry format from the E820 call
typedef struct __attribute__((packed)) {
    uint64_t base;
    uint64_t length;
    uint32_t type;
    uint32_t acpi_extended_attributes;
} e820_entry_t;

void init_pmm(uint8_t verbose);

uint64_t pmm_get_total_memory_mb();


#endif