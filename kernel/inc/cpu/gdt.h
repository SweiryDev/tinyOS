#ifndef __CPU_GDT
#define __CPU_GDT

#include <types.h>

// Represents a GDT entry
typedef struct __attribute__((packed)) {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_middle;
    uint8_t  access;
    uint8_t  limit_high_flags;
    uint8_t  base_high;
} gdt_entry_t;

// Represents a TSS entry (which is larger)
typedef struct __attribute__((packed)) {
    gdt_entry_t gdt_entry;
    uint32_t base_upper32;
    uint32_t reserved;
} tss_gdt_entry_t;



#endif