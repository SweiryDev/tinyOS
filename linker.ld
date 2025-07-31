/*
 * The entry point for our entire OS is the 'start' label
 * from the bootloader.
 */
ENTRY(start)

SECTIONS {

    . = 0x7C00;

    .bootloader : {
        boot (.text .data .rodata .bss)
    }

    /* No need for zero out paddings */
    . = 0x8200;

    .kernel : {
        entry.o (.text .data .rodata .bss)
        kernel.o(.text .data .rodata .bss)
        ports.o(.text .data .rodata .bss)
        vga.o(.text .data .rodata .bss)
    }
}