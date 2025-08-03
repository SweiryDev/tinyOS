#include <messages.h>
#include <cpu/ports.h>
#include <driver/vga.h>
#include <cpu/isr.h>
#include <cpu/pic.h>

// Initialize the kernel 
void init_kernel();
void div_by_zero();

int main(){
    init_kernel();

    div_by_zero(); // Test division by zero interrupt

    return 0;
}

void init_kernel(){
    // Install Interrupt Service Routine
    isr_install();
    irq_install();
    pic_remap();

    // Critical: Enable interrupts!
    __asm__ __volatile__ ("sti");

    // Clear screen and hide the cursor
    cleartext();
    hide_cursor();
    
    // Print kernel msg 
    putstr(kernel_msg);
    putstr(">");
}

void div_by_zero(){
    // Test Division by zero interrupts!
    // Define z=0 before division (else the compiler will set t=0)
    int z = 0;
    int t = 5/z;
    putchar(t + 48); // prints 0 ?
}
