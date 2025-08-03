# tinyOS C Kernel

This is the main 64-bit kernel for **tinyOS**. Control is handed off to the `main()` function in `src/kernel.c` after the assembly bootloader has successfully switched the CPU into 64-bit long mode.

The kernel is designed to be a monolithic, freestanding binary.

### Core Logic

* **`entry.asm`**: This is the first kernel file to be executed. Its only job is to set up a basic stack and then call the C `main()` function. This acts as the bridge between the assembly bootloader and the C kernel.
* **`src/kernel.c`**: The main high-level entry point for the kernel. It is responsible for initializing all the necessary drivers and systems.

### Features & Implementation Files

* **VGA Text Driver**: A complete C based driver for managing text output to the screen. It no longer relies on the BIOS and communicates directly with the VGA hardware.
    * **`inc/driver/vga.h`**: The public interface for the VGA driver, defining functions for printing, clearing, and cursor control.
    * **`src/driver/vga.c`**: The implementation of the VGA driver, including logic for screen scrolling.
    * **`src/cpu/ports.c`**: Contains the low-level `byte_in` and `byte_out` functions used by the VGA driver to communicate with hardware I/O ports.

* **Interrupt Handling System**: A robust system for handling both CPU exceptions and hardware interrupts, making the kernel stable and reactive.
    * **The IDT (`cpu/idt.h`, `cpu/idt.c`)**: Defines and manages the **Interrupt Descriptor Table**. `idt.c` contains the code to create the IDT and load it into the CPU with the `lidt` instruction.
    * **ISRs (`cpu/isr.h`, `cpu/isr.c`, `cpu/isr_defs.asm`)**: Handles interrupts. `isr_defs.asm` provides the low-level assembly stubs that save the CPU state and call the main C handler. `isr.c` contains the high-level C handlers that print descriptive error messages for CPU exceptions and manage hardware interrupts.
    * **The PIC (`cpu/pic.h`, `cpu/pic.c`)**: Contains the driver to **remap the Programmable Interrupt Controller (PIC)**. This is a critical step to prevent conflicts between hardware interrupts and CPU exceptions.

### Useful Links
* [OSDev Wiki: GDT](https://wiki.osdev.org/GDT)
* [OSDev Wiki: IDT](https://wiki.osdev.org/IDT)
* [OSDev Wiki: PIC](https://wiki.osdev.org/PIC)