# 16-bit to 64-bit Bootloader

This bootloader handles the complete startup sequence for **tinyOS**. It begins in 16-bit real mode, prepares the system, transitions to 32-bit protected mode, and finally enters 64-bit long mode, ready to hand off control to a C kernel.

### Stage 1: 16-bit Real Mode

This initial stage runs directly after being loaded by the BIOS. All operations here use BIOS interrupts.

* **Core Logic (`boot.asm`)**: The main file for the bootloader. The first 512 bytes are loaded by the BIOS, which then loads the subsequent sectors.
* **Multi-Sector Loading (`utils/load.asm`)**: Contains the `load_bios` function, which uses the `int 0x13` BIOS interrupt to read additional sectors from the boot drive.
* **Custom Splash Screen (`utils/splash.asm`, `utils/print.asm`)**: On boot, the OS clears the screen, hides the cursor, and displays a custom ASCII art logo, the current time, and the available memory.
* **Hardware Interfacing (BIOS)**: The bootloader gets system information using BIOS interrupts.
    * **Time Services (`utils/time.asm`)**: Reads the current time from the RTC using `int 0x1A`.
    * **Memory Services (`utils/memory.asm`)**: Detects the amount of conventional memory using `int 0x12`.
* **Interactive Prompt**: After displaying system info, the bootloader waits for the user to press **Enter** before proceeding, using `int 0x16`.

---

### Stage 2: 32-bit Protected Mode & 64-bit Long Mode

This stage handles the transition to modern CPU operating modes.

* **Global Descriptor Tables (GDT)**: Two GDTs are set up.
    * **`protected_mode/gdt32.asm`**: Defines the GDT required to enter 32-bit protected mode.
    * **`long_mode/gdt64.asm`**: Defines the GDT with a long mode flag, which is required to enter 64-bit mode.
* **Paging Setup (`protected_mode/paging32.asm`)**: Before entering long mode, a basic 4-level page table structure is created to identity-map the first 2MB of memory. This is a requirement for long mode.
* **Mode Switching (`protected_mode/switch32.asm`, `long_mode/switch64.asm`)**: These files contain the precise assembly sequences to load the GDTs, set the CPU control registers (`cr0`, `cr3`, `cr4`), and perform the far jumps needed to successfully transition into protected mode and then long mode.

### Useful Links
* [BIOS Interrupt Call (Wikipedia)](https://en.wikipedia.org/wiki/BIOS_interrupt_call)
* [OSDev Wiki: BIOS](https://wiki.osdev.org/BIOS)
* [OSDev Wiki: GDT](https://wiki.osdev.org/GDT)
* [OSDev Wiki: Paging](https://wiki.osdev.org/Paging)
* [OSDev Wiki: Long Mode](https://wiki.osdev.org/Long_Mode)