#include <cpu/tss.h>
#include <cpu/gdt.h>
#include <memory/vmm.h>
#include <utils/utils.h>
#include <driver/vga.h>

extern gdt_entry_t gdt64_address[];

// GDT selector for the TSS (from gdt64.asm)
#define TSS_SELECTOR 0x28

// Tss instance
static tss_t kernel_tss;


// Assembly function to load TSS register
void load_tss(uint16_t selector){
    __asm__ __volatile__("ltr %0" : : "r"(selector));
}

void init_tss(){
    uint64_t gdt_addr = (uint64_t)gdt64_address;

    // GDT starts at a known address loaded by the bootloader.
    tss_gdt_entry_t* tss_descriptor = (tss_gdt_entry_t*)(gdt_addr + TSS_SELECTOR);

    // Allocate stack for the kernel coming from user mode
    void *kernel_stack = vmm_alloc_page();
    uint64_t stack_top = (uint64_t)kernel_stack + 4096;
    
    // Zero out the TSS
    memset(&kernel_tss, 0, sizeof(tss_t));

    // Set the kernel stack pointer
    kernel_tss.rsp0 = stack_top;
    
    // POPULATE THE GDT DESCRIPTOR 
    uint64_t tss_base = (uint64_t)&kernel_tss;
    
    tss_descriptor->gdt_entry.base_low = tss_base & 0xFFFF;
    tss_descriptor->gdt_entry.base_middle = (tss_base >> 16) & 0xFF;
    tss_descriptor->gdt_entry.base_high = (tss_base >> 24) & 0xFF;
    tss_descriptor->base_upper32 = (tss_base >> 32) & 0xFFFFFFFF;
    
    tss_descriptor->gdt_entry.limit_low = sizeof(tss_t) - 1;

    // Load the TSS
    load_tss(TSS_SELECTOR);
}
