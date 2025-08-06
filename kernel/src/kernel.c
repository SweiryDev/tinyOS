#include <utils/messages.h>
#include <cpu/ports.h>
#include <driver/vga.h>
#include <cpu/isr.h>
#include <cpu/pic.h>
#include <memory/heap.h>
#include <utils/utils.h>
#include <shell/shell.h>
#include <driver/keyboard.h>


// Functions Signatures

// Initialize the kernel 
void init_kernel();

// Tests:
void div_by_zero();
void test_heap();
void print_formatted();


int main(){
    // Initialize the kernel
    init_kernel();


    return 0;
}

void init_kernel(){
    // Install Interrupt Service Routine
    isr_install();
    irq_install();
    pic_remap();
    init_heap(); 
    init_keyboard();

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

void test_heap(){
    putstr("Testing memory coalescing:\n");
    
    char* p1 = (char*) kmalloc(100);
    putstr("Allocated p1 (100 bytes).\n");

    char* p2 = (char*) kmalloc(150);
    putstr("Allocated p2 (150 bytes).\n");
    
    char* p3 = (char*) kmalloc(100);
    putstr("Allocated p3 (100 bytes).\n\n");
    
    putstr("Freeing p1, p2, and p3...\n");
    kfree(p1);
    kfree(p2);
    kfree(p3);
    
    char* p4 = (char*) kmalloc(350); // Should succeed now
    if (p4) {
        putstr("p4 allocated successfully (350 bytes).\n");
        putstr("Memory coalescing works!\n");
    } else {
        putstr("p4 allocation failed. Coalescing did not work.\n");
    }

    kfree(p4);
}

void print_formatted(){
    int x = 17847;
    // '%d'-Base 10, '%b'-Base 2, '%x'-Base 16, '%p'-Pointer address.
    putstrf("The Number Is: %b! ", &x);
}