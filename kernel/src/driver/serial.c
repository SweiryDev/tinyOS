#include <driver/serial.h>
#include <cpu/ports.h>

// I/O port addresses for the COM1 serial port
#define SERIAL_COM1_BASE                0x3F8

#define SERIAL_DATA_PORT(base)          (base)
#define SERIAL_FIFO_COMMAND_PORT(base)  (base + 2)
#define SERIAL_LINE_COMMAND_PORT(base)  (base + 3)
#define SERIAL_MODEM_COMMAND_PORT(base) (base + 4)
#define SERIAL_LINE_STATUS_PORT(base)   (base + 5)

// The I/O port commands

// Tells the serial port to expect the high byte of the baud rate divisor
#define SERIAL_LINE_ENABLE_DLAB         0x80

// COM1 serial port initialization
void init_serial() {
    // Disable all interrupts from the serial port
    byte_out(SERIAL_MODEM_COMMAND_PORT(SERIAL_COM1_BASE), 0x00);

    // Enable DLAB (Divisor Latch Access Bit) to set the baud rate
    byte_out(SERIAL_LINE_COMMAND_PORT(SERIAL_COM1_BASE), SERIAL_LINE_ENABLE_DLAB);

    // Set the baud rate divisor to 3 (38400 baud)
    // Low byte
    byte_out(SERIAL_DATA_PORT(SERIAL_COM1_BASE), 0x03);
    // High byte
    byte_out(SERIAL_DATA_PORT(SERIAL_COM1_BASE) + 1, 0x00);

    // Set the data format: 8 bits, no parity, one stop bit (8N1)
    byte_out(SERIAL_LINE_COMMAND_PORT(SERIAL_COM1_BASE), 0x03);

    // Enable FIFO, clear them, with 14-byte threshold
    byte_out(SERIAL_FIFO_COMMAND_PORT(SERIAL_COM1_BASE), 0xC7);

    // Re-enable interrupts
    byte_out(SERIAL_MODEM_COMMAND_PORT(SERIAL_COM1_BASE), 0x0B);
}

//  Checks if the transmit buffer is empty.
//  return 1 if the transmit buffer is empty, 0 otherwise.
static int is_transmit_empty() {
    // 0x20 is the bit that indicates if the transmit buffer is empty
    return byte_in(SERIAL_LINE_STATUS_PORT(SERIAL_COM1_BASE)) & 0x20;
}


//  Writes a single character to the serial port.
void serial_write(char a) {
    // Wait until the transmit buffer is empty
    while (is_transmit_empty() == 0);

    // Send the character
    byte_out(SERIAL_DATA_PORT(SERIAL_COM1_BASE), a);
}


// Writes a null-terminated string to the serial port.
void serial_print(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        serial_write(str[i]);
    }
}