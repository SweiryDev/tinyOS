/*
 * The entry point for our entire OS is the 'start' label
 * from the bootloader.
 */
ENTRY(start)

SECTIONS {

    /* Bootloader section starts at 0x7C00 (BIOS reading) */
    . = 0x7C00;

    .bootloader : {
        boot (.text .data .rodata .bss)
    }

    /* Kernel section starts at 1MB mark (for bootloader separation) */
    . = 0x10000;

    /* Include object files with sections of kernel and merge */
    .kernel : {
        *(.text*)
        *(.rodata*)
        *(.data*)
        *(.bss*)
    }

}