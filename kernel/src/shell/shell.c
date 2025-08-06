#include <shell/shell.h>
#include <driver/vga.h>
#include <utils/utils.h>
#include <types.h>
#include <utils/messages.h>

// Input buffer and index
#define CMD_BUFFER_SIZE 256
char cmd_buffer[CMD_BUFFER_SIZE];
uint16_t cmd_buffer_index = 0;

void process_command() {
    putstr("\n"); 

    // Commands
    if (strcmp(cmd_buffer, "help") == 0) {
        putstr("tinyOS v0.1. Commands: help, clear, fetch\n");
    } else if (strcmp(cmd_buffer, "clear") == 0) {
        cleartext();
        set_cursor_pos(0,0);
    } else if (strcmp(cmd_buffer, "") == 0){
        // No command
    } else if(strcmp(cmd_buffer, "fetch") == 0){
        putstr(kernel_msg);
        putstr(fetch_msg);
    } else {
        putstr("Unknown command: ");
        putstr(cmd_buffer);
        putstr("\n");
    }

    // Clear the buffer and print the prompt for the next command
    cmd_buffer_index = 0;
    memset(cmd_buffer, 0, CMD_BUFFER_SIZE);
    putstr(">");
}

// To be called from the keyboard driver
void shell_keyboard_handler(char key) {
    if (key == '\n') { // Enter key
        process_command();
    } else if (key == '\b') { // Backspace key
        if (cmd_buffer_index > 0) {
            cmd_buffer_index--;
            cmd_buffer[cmd_buffer_index] = '\0';
            // Also remove the character from the screen
            reverse_cursor();
            putchar(' ');
            reverse_cursor();
        }
    } else {
        // Add character to buffer if there is space
        if (cmd_buffer_index < CMD_BUFFER_SIZE - 1) {
            cmd_buffer[cmd_buffer_index++] = key;
            cmd_buffer[cmd_buffer_index] = '\0';
            // Echo the character to the screen
            putchar(key);
        }
    }
}

void init_shell() {
    putstr("Welcome to tinyOS!\n");
    putstr(">");
}