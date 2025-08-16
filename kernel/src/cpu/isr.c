#include <cpu/isr.h>
#include <cpu/idt.h>
#include <cpu/ports.h>
#include <cpu/syscall.h>

#include <driver/vga.h> // Print messages

#include <process/scheduler.h>

const char *exception_messages[] = {
    "Division By Zero", "Debug", "Non Maskable Interrupt", "Breakpoint",
    "Into Detected Overflow", "Out of Bounds", "Invalid Opcode", "No Coprocessor",
    "Double Fault", "Coprocessor Segment Overrun", "Bad TSS", "Segment Not Present",
    "Stack Fault", "General Protection Fault", "Page Fault", "Unknown Interrupt",
    "Coprocessor Fault", "Alignment Check", "Machine Check", "Reserved",
    "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved",
    "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved"};

// Interrupt handlers array
isr_t interrupt_handlers[256];

void register_interrupt_handler(uint8_t interrupt_number, isr_t handler)
{
    interrupt_handlers[interrupt_number] = handler;
}

// Timer interrupt (IRQ0) handler
static void timer_callback(context_t *context)
{
    schedule(context);
}

// Main C handler for assembly stubs call
void isr_handler(context_t *regs)
{

    // Check for system call interrupt
    if (regs->int_no == 128)
    {
        syscall_handler(regs);
        return;
    }

    putstr_color("Received Interrupt: ", COLOR_WHT, COLOR_RED);
    putstr_color(exception_messages[regs->int_no], COLOR_WHT, COLOR_RED);

    // Ignore for now...
    // Halt the system. In a real OS, we might try to recover.
    while (1)
        __asm__("hlt");
}

// Install ISRs into the IDT
void isr_install()
{
    set_idt_gate(0, (uint64_t)isr_0);
    set_idt_gate(1, (uint64_t)isr_1);
    set_idt_gate(2, (uint64_t)isr_2);
    set_idt_gate(3, (uint64_t)isr_3);
    set_idt_gate(4, (uint64_t)isr_4);
    set_idt_gate(5, (uint64_t)isr_5);
    set_idt_gate(6, (uint64_t)isr_6);
    set_idt_gate(7, (uint64_t)isr_7);
    set_idt_gate(8, (uint64_t)isr_8);
    set_idt_gate(9, (uint64_t)isr_9);
    set_idt_gate(10, (uint64_t)isr_10);
    set_idt_gate(11, (uint64_t)isr_11);
    set_idt_gate(12, (uint64_t)isr_12);
    set_idt_gate(13, (uint64_t)isr_13);
    set_idt_gate(14, (uint64_t)isr_14);
    set_idt_gate(15, (uint64_t)isr_15);
    set_idt_gate(16, (uint64_t)isr_16);
    set_idt_gate(17, (uint64_t)isr_17);
    set_idt_gate(18, (uint64_t)isr_18);
    set_idt_gate(19, (uint64_t)isr_19);
    set_idt_gate(20, (uint64_t)isr_20);
    set_idt_gate(21, (uint64_t)isr_21);
    set_idt_gate(22, (uint64_t)isr_22);
    set_idt_gate(23, (uint64_t)isr_23);
    set_idt_gate(24, (uint64_t)isr_24);
    set_idt_gate(25, (uint64_t)isr_25);
    set_idt_gate(26, (uint64_t)isr_26);
    set_idt_gate(27, (uint64_t)isr_27);
    set_idt_gate(28, (uint64_t)isr_28);
    set_idt_gate(29, (uint64_t)isr_29);
    set_idt_gate(30, (uint64_t)isr_30);
    set_idt_gate(31, (uint64_t)isr_31);

    // Register system call handler
    set_idt_gate(128, (uint64_t)isr_128);

    // Register the timer interrupt handler
    register_interrupt_handler(32, timer_callback);

    // Load IDT
    set_idt();
}

// C handler for all hardware interrupts
void irq_handler(context_t *regs)
{
    // Cast the regs pointer to the full context_t for the scheduler
    if (interrupt_handlers[regs->int_no] != 0)
    {
        isr_t handler = interrupt_handlers[regs->int_no];
        handler(regs);
    }

    // Send an End-of-Interrupt (EOI) signal to the PICs.
    // This is critical, otherwise the PIC won't send any more interrupts.
    if (regs->int_no >= 40)
    {
        byte_out(0xA0, 0x20); // EOI to slave PIC
    }
    byte_out(0x20, 0x20); // EOI to master PIC
}

// Install IRQ handlers to the IDT
void irq_install()
{
    set_idt_gate(32, (uint64_t)irq_0);
    set_idt_gate(33, (uint64_t)irq_1);
    set_idt_gate(34, (uint64_t)irq_2);
    set_idt_gate(35, (uint64_t)irq_3);
    set_idt_gate(36, (uint64_t)irq_4);
    set_idt_gate(37, (uint64_t)irq_5);
    set_idt_gate(38, (uint64_t)irq_6);
    set_idt_gate(39, (uint64_t)irq_7);
    set_idt_gate(40, (uint64_t)irq_8);
    set_idt_gate(41, (uint64_t)irq_9);
    set_idt_gate(42, (uint64_t)irq_10);
    set_idt_gate(43, (uint64_t)irq_11);
    set_idt_gate(44, (uint64_t)irq_12);
    set_idt_gate(45, (uint64_t)irq_13);
    set_idt_gate(46, (uint64_t)irq_14);
    set_idt_gate(47, (uint64_t)irq_15);
}
