#ifndef __CPU_TSS
#define __CPU_TSS

#include <types.h>


// Task State Segment (TSS) structure
typedef struct __attribute__((packed)) {
    uint32_t reserved1;
    uint64_t rsp0; // The stack pointer to load when switching to Ring 0
    uint64_t rsp1;
    uint64_t rsp2;
    uint64_t reserved2;
    uint64_t ist1;
    uint64_t ist2;
    uint64_t ist3;
    uint64_t ist4;
    uint64_t ist5;
    uint64_t ist6;
    uint64_t ist7;
    uint64_t reserved3;
    uint16_t reserved4;
    uint16_t iopb_offset;
} tss_t;

void init_tss();

#endif