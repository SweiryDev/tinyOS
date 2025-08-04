#include <driver/keyboard.h>
#include <driver/vga.h>
#include <cpu/ports.h>
#include <cpu/isr.h>
#include <types.h>

static uint8_t shift_pressed = 0;

// A basic US keyboard layout scan code map
const char scancode_map[] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-',
    0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

// Map for shift hold keys
const char shift_scancode_map[] = {
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
    0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,
    '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-',
    0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

// The keyboard interrupt handler
static void keyboard_callback(registers_t *regs) {
    // Read the scan code from the keyboard's data port
    uint8_t scancode = byte_in(0x60);
    
    switch(scancode){
        case 0x2A: // left shift press
            shift_pressed = 1;
            return;

        case 0x36: // right shift press
            shift_pressed = 1;
            return;

        case 0xAA: // left shift release
            shift_pressed = 0;
            return;

        case 0xB6: // right shift release
            shift_pressed = 0;
            return;

    }

    // Ignore key release codes 
    if (scancode & 0x80) {
        return;
    }

    // Select map based on shift state
    const char* map = shift_pressed ? shift_scancode_map : scancode_map;

    // Translate the scan code to ASCII and print it
    char ascii_char = map[scancode];
    
    if (ascii_char) {
        char str[2] = {ascii_char, 0};
        putstr_color(str, DEFAULT_COLOR_FG, DEFAULT_COLOR_BG);
    }
}

// Initializes the keyboard driver
void init_keyboard() {
    // Register keyboard_callback to handle keyboard interrupts
    register_interrupt_handler(33, keyboard_callback);
}