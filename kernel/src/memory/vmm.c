#include <memory/vmm.h>
#include <memory/pmm.h>
#include <utils/utils.h>
#include <driver/vga.h>


// Top-level page table (PML4) for the kernel
// Align this to a 4KB boundary
__attribute__((aligned(4096)))
static page_table_t kernel_pml4[1];

// Map a single 4KB virtual page to a physical page.
void vmm_map_page(void* virtual_address, void* physical_address){
    uint64_t virt_addr = (uint64_t)virtual_address;

    // Address mapping debugging:
    // if(physical_address > (uint64_t)0x3F000000) putstrf("DEBUG: addr %p\n", physical_address);
    
    // Calculate the (9 bits) indices for each level of the page table
    // from the virtual address
    uint64_t pml4_index = (virt_addr >> 39) & 0x1FF;
    uint64_t pdpt_index = (virt_addr >> 30) & 0x1FF;
    uint64_t pdt_index = (virt_addr >> 21) & 0x1FF;
    uint64_t pt_index = (virt_addr >> 12) & 0x1FF;

    // Walk the PML4 table
    pte_t* pml4e = &kernel_pml4[0][pml4_index];
    page_table_t* pdpt;
    if(!(*pml4e & PTE_PRESENT)){
        // PDPT doesn't exist, create it
        pdpt = (page_table_t*)pmm_alloc_page();
        memset(pdpt, 0, 4096); // Zero out the new page table
        // Link the new PDPT into the PML4, mark as present and writable
        *pml4e = (pte_t)pdpt | PTE_PRESENT | PTE_READ_WRITE;
    } else {
        // PDPT already set, fetch its address
        pdpt = (page_table_t*)(*pml4e & ~0xFFF); // Mask out flags to get the address
    }

    // Walk the PDPT table
    pte_t* pdpte = &(*pdpt)[pdpt_index];
    page_table_t* pdt;
    if(!(*pdpte & PTE_PRESENT)){
        // PDT doesn't exist, create it
        pdt = (page_table_t*)pmm_alloc_page();
        memset(pdt, 0, 4096);
        *pdpte = (pte_t)pdt | PTE_PRESENT | PTE_READ_WRITE;
    } else {
        pdt = (page_table_t*)(*pdpte & ~0xFFF);
    }

    // Walk the PDT table
    pte_t* pdte = &(*pdt)[pdt_index];
    page_table_t* pt;
    if(!(*pdte & PTE_PRESENT)){
        // PT doesn't exist create it
        pt = (page_table_t*)pmm_alloc_page();
        memset(pt, 0, 4096);
        *pdte = (pte_t)pt | PTE_PRESENT | PTE_READ_WRITE;
    } else {
        pt = (page_table_t*)(*pdte & ~0xFFF);
    }
    
    // Set the entry in the page table to the physical page
    pte_t* pte = &(*pt)[pt_index];
    *pte = (pte_t)physical_address | PTE_PRESENT | PTE_READ_WRITE;
}

// Initialize Virtual Memory Manager
void init_vmm(){
    // Map bootloader (low memory)
    for(uint64_t addr=0; addr<0x9FC00; addr+=4096){
        vmm_map_page((void*)addr, (void*)addr);
    }

    // Map VGA BUFFER
    for(uint64_t addr=VGA_START; addr<(VGA_START + VGA_EXTENT); addr+=4096){
        vmm_map_page((void*)addr, (void*)addr);
    }

    // Map kernel
    // Get the memory map from the addresses set by the bootloader
    uint16_t entry_count = (*(uint16_t*)0x8E00);
    e820_entry_t* mem_map = (e820_entry_t*)0x9000;

    // Iterate through every entry in the map
    for (uint16_t i = 0; i < entry_count; i++) {
        e820_entry_t* entry = &mem_map[i];
        // If the entry is marked "Usable" (Type 1), map the entire region
        // and isn't the low memory
        if (entry->type == 1 && entry->base != 0) {
            uint64_t length_mb = entry->length/(1024*1024);
            putstrf("Entry Length: %dMB", &(length_mb));
            // Map virtual memory for the kernel
            for (uint64_t addr = entry->base; addr < (entry->base + entry->length); addr += 4096) {
                vmm_map_page((void*)addr, (void*)addr);
            }
        }
    }
}

// Load PML4 into the CR3 register
void vmm_activate(){
    // Physical address of PML4 table
    uint64_t pml4_phys_addr = (uint64_t)&kernel_pml4;

    // Load address into CR3
    __asm__ __volatile__ ("mov %0, %%cr3" : : "r" (pml4_phys_addr));
}

void* vmm_alloc_page(){
    // Allocate a free physical page from the pmm
    void* phys_addr = pmm_alloc_page();

    // Out of memory ?
    if(!phys_addr){
        putstr("NO VIRTUAL MEMORY TO ALLOCATE");
        return 0;
    }

    // For the kernel (virt_addr = phys_addr)
    vmm_map_page(phys_addr, phys_addr);

    return phys_addr;
}
