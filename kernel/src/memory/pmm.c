#include <memory/pmm.h>
#include <driver/serial.h>
#include <utils/utils.h>
#include <driver/vga.h>

// Addresses where the map stored in boot.asm
#define E820_MAP_ADDR ((e820_entry_t*)0x9000)
#define E820_COUNT_ADDR (*(uint16_t*)0x8E00)

// External symbols from the linker script
extern uint64_t _kernel_start;
extern uint64_t _kernel_end;

static uint8_t* pmm_bitmap = 0;
static uint64_t pmm_bitmap_size = 0;
static uint64_t total_memory_pages = 0;

// Static variable to store the total amount of usable memory in KB
static uint64_t total_memory_kb = 0;

// A helper array of strings to make the output readable
const char* E820_TYPE_STRINGS[] = {
    "Invalid",
    "Usable",
    "Reserved",
    "ACPI Reclaimable",
    "ACPI NVS",
    "Bad Memory"
};

void pmm_print_map() {
    uint16_t entry_count = E820_COUNT_ADDR;
    e820_entry_t *mem_map = E820_MAP_ADDR;

    putstr("\n--- Memory Map (BIOS) ---\n");
    putstr("Entry Count: ");
    putstr(itoa(entry_count, 10));
    putstr("\n");

    for (uint16_t i = 0; i < entry_count; i++) {
        e820_entry_t* entry = &mem_map[i];
        const char* type_str = (entry->type < 6) ? E820_TYPE_STRINGS[entry->type] : E820_TYPE_STRINGS[2];

        putstr("Base: 0x");
        putstr(itoa(entry->base, 16));
        putstr(" | Length: 0x");
        putstr(itoa(entry->length, 16));
        putstr(" | Type: ");
        putstr(type_str);
        putstr("\n");
    }
    putstr("-----------------------\n");
}

void pmm_print_kernel_info(){
    uint64_t kernel_start_addr = (uint64_t)&_kernel_start;
    uint64_t kernel_end_addr = (uint64_t)&_kernel_end;
    uint64_t kernel_size_bytes = kernel_end_addr - kernel_start_addr;
    uint64_t kernel_size_kb = kernel_size_bytes / 1024;

    putstr("\n-- Kernel Info --\n");
    putstr("Kernel Start: 0x");
    putstr(itoa(kernel_start_addr, 16));
    putstr("\nKernel End:   0x");
    putstr(itoa(kernel_end_addr, 16));
    putstr("\nKernel Size:  ");
    putstr(itoa(kernel_size_kb, 10));
    putstr(" KB\n");
    putstr("-------------------\n");

}

void init_pmm() {
    uint16_t entry_count = E820_COUNT_ADDR;
    e820_entry_t* mem_map = E820_MAP_ADDR;
    uint64_t total_usable_memory_bytes = 0;

    // Find the largest usable memory region
    e820_entry_t* largest_region = 0;
    for (uint16_t i = 0; i < entry_count; i++) {
        e820_entry_t* entry = &mem_map[i];
        if (entry->type == 1) { // Type 1 is "Usable"
            total_usable_memory_bytes += entry->length;
            if (largest_region == 0 || entry->length > largest_region->length) {
                largest_region = entry;
            }
        }
    }

    // Calculate total usable memory in KB
    total_memory_kb = total_usable_memory_bytes / 1024;

    // Calculate bitmap size
    uint64_t total_pages = total_usable_memory_bytes / 4096;
    pmm_bitmap_size = total_pages / 8;
    if (pmm_bitmap_size * 8 < total_pages) pmm_bitmap_size++;


    // Place the bitmap
    if (largest_region != 0) {
        pmm_bitmap = (uint8_t*)largest_region->base;
    }
        
    // Mark all memory as used by default (of the bitmap)
    memset(pmm_bitmap, 0xFF, pmm_bitmap_size);

    // Mark usable memory regions as free by clearing bits
    for(uint16_t i=0; i<entry_count; i++){
        // Iterate entries
        e820_entry_t* entry = &mem_map[i];
        if(entry->type == 1){ // find "Usable" regions
            for(uint64_t j=0; j < entry->length; j+=4096){
                // Calculate page number
                uint64_t page_num = (entry->base + j) / 4096;
                // Clear bits for this page in the bitmap
                pmm_bitmap[page_num/8] &= ~(1 << (page_num % 8));
            }
        }
    } 

    // Reserve memory used by the kernel, bios, and bitmap
    uint64_t kernel_start_addr = (uint64_t)&_kernel_start;
    uint64_t kernel_end_addr = (uint64_t)&_kernel_end;

    for(uint64_t addr=0; addr<kernel_end_addr; addr+=4096){
        uint64_t page_num = addr/4096;
        // Set bit for this page (used)
        pmm_bitmap[page_num/8] |= (1 << (page_num % 8));
    }

    // Reserve the bitmap pages
    uint64_t bitmap_start_addr = (uint64_t)pmm_bitmap;
    uint64_t bitmap_end_addr = bitmap_start_addr + pmm_bitmap_size;
    for(uint64_t addr=bitmap_start_addr; addr<bitmap_end_addr; addr+=4096){
        uint64_t page_num = addr/4096;
        // Set the bit for this page (used)
        pmm_bitmap[page_num/8] |= (1 << (page_num % 8));
    }

}

// Total usable memory in megabytes.
uint64_t pmm_get_total_memory_mb() {
    return total_memory_kb / 1024;
}

// Allocate a single 4KB page of physical memory
// Return a pointer to the start of the allocated page (or 0 if no free space)
void* pmm_alloc_page(){
    // Iterate the entire bitmap
    for(uint64_t i=0; i<pmm_bitmap_size; i++){
        // Check for available page (!= 0xFF), iterate bytes
        if(pmm_bitmap[i] != 0xFF){
            // Iterate bits of this byte
            for(uint8_t j=0; j<8; j++){
                // Find free page (0 bit)
                if(!(pmm_bitmap[i] & (1<<j))){
                    // Calculate page number
                    uint64_t page_num = i*8 + j;

                    // Mark as used (set bit)
                    pmm_bitmap[i] |= (1<<j);

                    // Return physical address of page start
                    return (void*)(page_num * 4096);
                }
            }
        }
    }

    // No free pages
    return 0;
}

// Free allocated 4KB page
void pmm_free_page(void* ptr){
    // Calculate page number
    uint64_t page_num = (uint64_t)ptr/4096;
    
    // Mark as free (clear bit)
    pmm_bitmap[page_num/8] &= ~(1 << (page_num % 8));
}

