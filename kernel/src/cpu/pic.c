#include <cpu/ports.h>
#include <cpu/pic.h>
#include <types.h>

#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1
#define ICW1_INIT    0x11
#define ICW4_8086    0x01

void pic_remap(void) {
    uint8_t a1 = byte_in(PIC1_DATA);
    uint8_t a2 = byte_in(PIC2_DATA);
    byte_out(PIC1_COMMAND, ICW1_INIT);
    byte_out(PIC2_COMMAND, ICW1_INIT);
    byte_out(PIC1_DATA, 0x20); // Master PIC vector offset
    byte_out(PIC2_DATA, 0x28); // Slave PIC vector offset
    byte_out(PIC1_DATA, 4);
    byte_out(PIC2_DATA, 2);
    byte_out(PIC1_DATA, ICW4_8086);
    byte_out(PIC2_DATA, ICW4_8086);
    byte_out(PIC1_DATA, a1);
    byte_out(PIC2_DATA, a2);
}