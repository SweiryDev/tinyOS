#include <cpu/ports.h>
#include <cpu/isr.h>
#include <cpu/pic.h>

#include <utils/messages.h>
#include <utils/utils.h>

#include <driver/vga.h>
#include <driver/serial.h>
#include <driver/keyboard.h>
#include <driver/rtc.h>

#include <memory/heap.h>
#include <memory/pmm.h>
#include <memory/vmm.h>

#include <process/scheduler.h>

#include <shell/shell.h>



// -- Functions Signatures --
// Initialize the kernel 
void init_kernel();

// Tests:
void div_by_zero();
void test_heap();
void print_formatted();
void print_serial();
void print_rtc();
void test_pmm();
// ----

int main(){
    // Initialize the kernel
    init_kernel();

    return 0;
}

void init_kernel(){
    // -- Installation and initialization --

    // Interrupts installation and remmaping
    isr_install();
    irq_install();
    pic_remap();

    // Serial port initialization
    init_serial();

    // Heap memory initialization
    init_heap(); 

    // Physical memory and virtual memory initialization
    init_pmm();
    init_vmm();

    // Initiate scheduler 
    // Critical: before vmm activation
    init_scheduler();

    // Activate virtual memory 
    vmm_activate();

    // Keyboard driver initialization
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

void print_serial(){
    // COM1 serial port print
    serial_print("tinyOS Serial Driver Initialized!\n");
}

void print_rtc() {
    // Define time structure and read time and date
    rtc_time_t current_time;
    rtc_read_time(&current_time);

    // Format time and date to string
    char *timestr = time_string(&current_time);
    char *datestr = date_string(&current_time);
    
    // Print time and date
    putstrf("\nTime: %s", timestr);
    putstrf(" | Date: %s", datestr);

    // Free...
    kfree(timestr);
    kfree(datestr);

    // Print to serial
    serial_print("RTC Time: ");
    serial_print(itoa(current_time.hour, 10));
    serial_print(":");
    serial_print(itoa(current_time.minute, 10));
    serial_print(":");
    serial_print(itoa(current_time.second, 10));

    serial_print(" | Date: ");
    serial_print(itoa(current_time.day, 10));
    serial_print("/");
    serial_print(itoa(current_time.month, 10));
    serial_print("/");
    serial_print(itoa(current_time.year, 10));
    serial_print("\n");

}

void test_pmm() {
    putstr("\n--- PMM Allocation Test ---\n");
    // Allocate a page
    void* p1 = pmm_alloc_page();

    if(p1 == 0){
        putstr("No Free Space!");
        return;
    }

    putstr("Allocated p1 at: 0x");
    putstr(itoa((uint64_t)p1, 16));
    putstr("\n");

    // Allocate another page
    void* p2 = pmm_alloc_page();

    if(p2 == 0){
        putstr("No Free Space!");
        return;
    }

    putstr("Allocated p2 at: 0x");
    putstr(itoa((uint64_t)p2, 16));
    putstr("\n");

    // Free the first page
    putstr("Freeing p1...\n");
    pmm_free_page(p1);

    // Allocate a third page
    void* p3 = pmm_alloc_page();

    if(p3 == 0){
        putstr("No Free Space!");
        return;
    }

    putstr("Allocated p3 at: 0x");
    putstr(itoa((uint64_t)p3, 16));
    putstr(" (should be same as p1)\n");
    putstr("--------------------------\n");
}
