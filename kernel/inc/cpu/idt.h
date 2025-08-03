#ifndef __IDT
#define __IDT

#include <types.h>

// Code segment selector to load from the GDT
#define KERNEL_CS 0x08

// Attribute and entries 
#define INT_ATTR 0x8E           // 0b10001110
#define IDT_ENTRIES 256


// Interrupt handler register
typedef struct __attribute__((packed)){
    uint16_t limit; // Total size of the IDT
    uint64_t base;  // Start memory address of the IDT
} idt_register;

// Interrupt handler gate (IDT entry)
typedef struct __attribute__((packed)) {
    uint16_t base_low;             // Low 16 bits of the address to jump to
    uint16_t cs_selector;          // Code segment selector
    uint8_t  zero;                 
    uint8_t  attributes;           // Flag bytes
                                //      - Bit 7:     Interrupt is present
                                //      - Bits 6-5:  Privelege level of caller (0=kernel..3=user)
                                //      - Bit 4:     Set to 0 for interrupt gates
                                //      - Bits 3-0:  Always 1110 for 32 bit interrupt gate
    uint16_t base_middle;          // Middle 16 bits of the address to jump to
    uint32_t base_high;            // High 16 bits of the address to jump to
    uint32_t reserved;             
} idt_gate;

// Make IDT active
void set_idt();

// Format and set IDT entry
void set_idt_gate(uint8_t gate_number, uint64_t handler_address);

#endif