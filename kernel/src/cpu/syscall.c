#include <cpu/syscall.h>
#include <driver/vga.h>
#include <utils/utils.h>

// Main dispatcher function
void syscall_handler(context_t* regs){
    // System call number from the RAX register
    syscall_number_t syscall_num = (syscall_number_t)regs->rax;

    switch(syscall_num){        
        case SYS_YIELD:
            // Manually trigger the timer interrupt (IRQ 0, interrupt 32)
            // invoke the scheduler
            __asm__ __volatile__ ("int $32");
            break;

        case SYS_WRITE_CONSOLE:
            // First argument in the RDI
            putstrf("%s", (const char*)regs->rdi);
            break;

        default:
            putstr_color("Unknown syscall number: ", COLOR_RED, COLOR_BLK);
            putstr(itoa((uint64_t)syscall_num, 10));
            putstr("\n");
            break;
    }

}

// Wrapper for making a syscall
void syscall(syscall_number_t syscall_num, ...){
    __asm__ __volatile(
        "mov %0, %%rax\n"   // Move the syscall number into RAX
        "int $0x80"         // Trigger the interrupt
        : // No output
        : "g"(syscall_num)  // Input syscall number
        : "rax"             // Clobbered register (RAX is modified)
    );
}

// sys_write_console wrapper
void sys_write_console(const char* str){
    __asm__ __volatile__ (
        "mov %0, %%rax\n"
        "mov %1, %%rdi\n" // string pointer argument
        "int $0x80"
        :
        : "g"(SYS_WRITE_CONSOLE), "g"(str)
        : "rax", "rdi"
    );
}
