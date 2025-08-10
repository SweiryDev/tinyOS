#include <shell/shell.h>
#include <driver/vga.h>
#include <utils/utils.h>
#include <types.h>
#include <utils/messages.h>
#include <driver/rtc.h>
#include <memory/heap.h>
#include <memory/pmm.h>

// Input buffer and index
#define CMD_BUFFER_SIZE 256
char cmd_buffer[CMD_BUFFER_SIZE];
uint16_t cmd_buffer_index = 0;

void process_command() {
    putstr("\n"); 

    // Commands
    if (strcmp(cmd_buffer, "help") == 0) {
        // Print help message
        putstr(help_msg);

    } else if (strcmp(cmd_buffer, "clear") == 0) {
        // Clear shell
        cleartext();
        set_cursor_pos(0,0);
    
    } else if (strcmp(cmd_buffer, "") == 0){
        // No command
    
    } else if(strcmp(cmd_buffer, "fetch") == 0){
        // Print Fetch messages
        putstr(kernel_msg);
        putstr(fetch_msg);
        
        // Get total memory in KB
        uint64_t total_mem_mb = pmm_get_total_memory_mb();
        char* mem_str = itoa(total_mem_mb, 10);
        
        // Print the information
        putstrf("Memory: %s KB\n", mem_str);

        // Free the allocated string
        kfree(mem_str);
    
    } else if(strcmp(cmd_buffer, "time") == 0){
        // Print time and dat from the RTC
        rtc_time_t current_time;
        rtc_read_time(&current_time);

        char *timestr = time_string(&current_time);
        char *datestr = date_string(&current_time);
        putstrf("Time: %s", timestr);
        putstrf(" | Date: %s\n", datestr);

        kfree(timestr);
        kfree(datestr);
    
    } else if(strcmp(cmd_buffer, "memorymap") == 0) {
        // Print memory map from BIOS
        pmm_print_map();
        pmm_print_kernel_info();

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