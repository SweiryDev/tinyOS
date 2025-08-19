#include <types.h>

#include <cpu/ports.h>

#include <shell/shell.h>

#include <utils/utils.h>
#include <utils/messages.h>

#include <driver/rtc.h>
#include <driver/vga.h>

#include <memory/heap.h>
#include <memory/pmm.h>

// Input buffer and index
#define CMD_BUFFER_SIZE 256
char cmd_buffer[CMD_BUFFER_SIZE];
uint16_t cmd_buffer_index = 0;

// Define port and value for QEMU 
// other emulators or real hardware might need
// another port and value to shutdown
#define QEMU_SHUTDOWN_PORT 0x604
#define QEMU_SHUTDOWN_VALUE 0x2000

void process_command() {
    clear_last_line_clock();
    putstr("\n"); 

    // Trim leading whitespace
    int i = 0, z = 0;
    while (cmd_buffer[i] == ' ')
        i++;
    while ((cmd_buffer[z++] = cmd_buffer[i++]));

    // Commands
    if (!strcmp(cmd_buffer, "help")) {
        // Print help message
        putstr(help_msg);

    } else if (!strcmp(cmd_buffer, "clear")) {
        // Clear shell
        cleartext();
        set_cursor_pos(0,0);
    
    } else if (!strcmp(cmd_buffer, "")){
        // No command
    
    } else if(!strcmp(cmd_buffer, "fetch")){
        // Print Fetch messages
        putstr(fetch_msg);
        putstr("---------------------\n");
        
        // Get total memory in KB
        uint64_t total_mem_mb = pmm_get_total_memory_mb();
        putstrf("Total memory: %dMB\n", &total_mem_mb);
        
        putstr("---------------------\n");
        

    } else if(!strcmp(cmd_buffer, "time")){
        // Print time and dat from the RTC
        rtc_time_t current_time;
        rtc_read_time(&current_time);

        char *timestr = time_string(&current_time);
        char *datestr = date_string(&current_time);
        putstrf("Time: %s", timestr);
        putstrf(" | Date: %s\n", datestr);

        kfree(timestr);
        kfree(datestr);
    
    } else if(!strcmp(cmd_buffer, "memorymap")) {
        // Print memory map from BIOS
        pmm_print_map();
        pmm_print_kernel_info();

    } else if(!strcmp(cmd_buffer, "shutdown")){

        putstr("Shutting down...\n");
        word_out(QEMU_SHUTDOWN_PORT, QEMU_SHUTDOWN_VALUE);
    
    } else {
        // Unknown command...
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
