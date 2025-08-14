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

    /* Page tables starts at 0x10000 up to 0x19000 */

    /* Kernel section starts at 0x19000 address (for bootloader separation) */
    . = 0x19000;

    /* Symbol of physical start address of the kernel */
    _kernel_start = .;

    /* Include object files with sections of kernel and merge */
    .kernel : {
        *(.text*)
        *(.rodata*)
        *(.data*)
        *(.bss*)
    }

    /* Symbol of physical end address of the kernel */
    _kernel_end = .;

}