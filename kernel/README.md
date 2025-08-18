# tinyOS C Kernel

This is the main 64-bit kernel for **tinyOS**. Control is handed off to the `main()` function in `src/kernel.c` after the assembly bootloader has successfully switched the CPU into 64-bit long mode.

The kernel is designed to be a freestanding binary.

### Core Logic

* **`entry.asm`**: This is the first kernel file to be executed. Its only job is to set up a basic stack and then call the C `main()` function. This acts as the bridge between the assembly bootloader and the C kernel.
* **`src/kernel.c`**: The main high-level entry point for the kernel. It is responsible for initializing all the necessary drivers and systems, including interrupt handling, memory management, and the scheduler.

### Features & Implementation Files

* **Memory Management**: A complete memory management system that includes physical, virtual, and heap memory allocation.
    * **Physical Memory Manager (PMM)**: Uses a bitmap to manage physical memory pages, initialized based on the memory map provided by the BIOS (E820).
        * `inc/memory/pmm.h`: The public interface for the PMM.
        * `src/memory/pmm.c`: Implementation of the bitmap-based physical page allocator.
    * **Virtual Memory Manager (VMM)**: Implements 4-level paging to map virtual addresses to physical addresses.
        * `inc/memory/vmm.h`: Defines the page table entry structure and mapping functions.
        * `src/memory/vmm.c`: The implementation for creating and managing page tables.
    * **Heap Allocator**: A simple block-based dynamic memory allocator for the kernel.
        * `inc/memory/heap.h`: Defines the `kmalloc` and `kfree` functions.
        * `src/memory/heap.c`: The implementation of the kernel's heap.

* **Interrupt Handling System**: A robust system for handling both CPU exceptions and hardware interrupts, making the kernel stable and reactive.
    * **The IDT (`cpu/idt.h`, `cpu/idt.c`)**: Defines and manages the **Interrupt Descriptor Table**. `idt.c` contains the code to create the IDT and load it into the CPU with the `lidt` instruction.
    * **ISRs (`cpu/isr.h`, `cpu/isr.c`, `cpu/isr_defs.asm`)**: Handles interrupts. `isr_defs.asm` provides the low-level assembly stubs that save the CPU state and call the main C handler. `isr.c` contains the high-level C handlers that manage hardware interrupts and print descriptive messages for CPU exceptions.
    * **The PIC (`cpu/pic.h`, `cpu/pic.c`)**: Contains the driver to **remap the Programmable Interrupt Controller (PIC)**. This is a critical step to prevent conflicts between hardware interrupts and CPU exceptions.

* **Process Management**: A basic cooperative multitasking system.
    * **Scheduler**: A simple round-robin scheduler that switches between tasks on each timer interrupt.
        * `inc/process/scheduler.h`: The public interface for the scheduler.
        * `src/process/scheduler.c`: The implementation of the task switching logic.
    * **Task Structure**: Defines the structure for a task, which includes its saved CPU context.
        * `inc/process/task.h`: The definition of `task_t` and `context_t`.

* **Drivers**: A collection of drivers for interacting with hardware devices.
    * **VGA Text Driver**: A C-based driver for managing text output to the screen, communicating directly with the VGA hardware.
        * `inc/driver/vga.h`: The public interface for printing, clearing, and cursor control.
        * `src/driver/vga.c`: The implementation of the VGA driver logic.
    * **Keyboard Driver**: Handles keyboard input by processing scan codes from the keyboard controller.
        * `inc/driver/keyboard.h`: The public interface for the keyboard driver.
        * `src/driver/keyboard.c`: The interrupt handler and scan code mapping for a US keyboard layout.
    * **RTC Driver**: Reads the current date and time from the Real-Time Clock chip.
        * `inc/driver/rtc.h`: The public interface for the RTC driver.
        * `src/driver/rtc.c`: The implementation for reading and formatting time from the RTC.
    * **Serial Driver**: A simple driver for communicating over the COM1 serial port, useful for debugging.
        * `inc/driver/serial.h`: The public interface for the serial driver.
        * `src/driver/serial.c`: The implementation for initializing and writing to the serial port.

* **System Calls**: A basic system call interface for user-space applications (though none exist yet).
    * `inc/cpu/syscall.h`: Defines the system call numbers and handler functions.
    * `src/cpu/syscall.c`: The main syscall dispatcher that is triggered by interrupt `0x80`.

* **Shell**: A simple, interactive command-line interface for the kernel.
    * `inc/shell/shell.h`: The public interface for the shell.
    * `src/shell/shell.c`: The implementation of the command processing and handling user input.

### Useful Links
* [OSDev Wiki: GDT](https://wiki.osdev.org/GDT)
* [OSDev Wiki: IDT](https://wiki.osdev.org/IDT)
* [OSDev Wiki: PIC](https://wiki.osdev.org/PIC)
* [OSDev Wiki: Paging](https://wiki.osdev.org/Paging)
* [OSDev Wiki: Physical Memory Management](https://wiki.osdev.org/PMM)
* [OSDev Wiki: Kernel Heap](https://wiki.osdev.org/Kernel_Heap)
* [OSDev Wiki: System Calls](https://wiki.osdev.org/System_Calls)
* [OSDev Wiki: RTC](https://wiki.osdev.org/RTC)