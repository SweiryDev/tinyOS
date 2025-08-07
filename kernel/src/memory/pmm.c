#include <memory/pmm.h>
#include <driver/serial.h>
#include <utils/utils.h>
#include <driver/vga.h>

// Addresses where the map stored in boot.asm
#define E820_MAP_ADDR ((e820_entry_t*)0x9000)
#define E820_COUNT_ADDR (*(uint16_t*)0x8E00)

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

void init_pmm(uint8_t verbose) {
    uint16_t entry_count = E820_COUNT_ADDR;
    e820_entry_t *mem_map = E820_MAP_ADDR;
    uint64_t total_usable_memory_bytes = 0;


    if(verbose){
        putstr("\n--- Memory Map (BIOS) ---\n");
        putstr("Entry Count: ");
        putstr(itoa(entry_count, 10));
        putstr("\n");
    }

    for (uint16_t i = 0; i < entry_count; i++) {
        e820_entry_t* entry = &mem_map[i];
        const char* type_str = (entry->type < 6) ? E820_TYPE_STRINGS[entry->type] : E820_TYPE_STRINGS[2];

        // If the type is "Usable" (Type 1), add its length to our total
        if (entry->type == 1) {
            total_usable_memory_bytes += entry->length;
        }

        if(verbose){
            putstr("Base: 0x");
            putstr(itoa(entry->base, 16));
            putstr(" | Length: 0x");
            putstr(itoa(entry->length, 16));
            putstr(" | Type: ");
            putstr(type_str);
            putstr("\n");
        }
    }

    total_memory_kb = total_usable_memory_bytes / 1024;

    if(verbose){
        putstr("-----------------------\n");

        // Convert the total from bytes to kilobytes and store it
        putstr("Total Usable Memory: ");
        putstr(itoa((uint64_t)total_memory_kb/1024, 10));
        putstr(" MB\n\n");
    }
}


// Total usable memory in megabytes.
uint64_t pmm_get_total_memory_mb() {
    return total_memory_kb / 1024;
}

