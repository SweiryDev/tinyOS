#include <cpu/idt.h>

// Define the IDT with 256 entries
idt_gate main_idt[IDT_ENTRIES];
// Define the pointer to the IDT
idt_register main_idt_reg;

// Load the IDT into the CPU
void set_idt() {
    main_idt_reg.base = (uint64_t)&main_idt;
    main_idt_reg.limit = (IDT_ENTRIES * sizeof(idt_gate)) - 1;
    // Load Interrupt Descriptor Table (lidt)
    __asm__ __volatile__ ("lidt (%0)" : : "r" (&main_idt_reg));
}

// Set entry in the IDT
void set_idt_gate(uint8_t gate_number, uint64_t handler_address) {
    main_idt[gate_number].base_low = (uint16_t)(handler_address & 0xFFFF);
    main_idt[gate_number].cs_selector = KERNEL_CS;
    main_idt[gate_number].zero = 0;
    main_idt[gate_number].attributes = INT_ATTR;
    main_idt[gate_number].base_middle = (uint16_t)((handler_address >> 16) & 0xFFFF);
    main_idt[gate_number].base_high = (uint32_t)((handler_address >> 32) & 0xFFFFFFFF);
    main_idt[gate_number].reserved = 0;
}