# Bootloader

BIOS Interrupts and functions are specified in (BIOS Wikipedia)[https://en.wikipedia.org/wiki/BIOS_interrupt_call] and (BIOS OSDev)[https://wiki.osdev.org/BIOS]

Interrupt 0x10 is used for the video services and used to print (register ah=0x0E), and clean (ax=0x03) the screen.
Video services is described in the print.asm file.