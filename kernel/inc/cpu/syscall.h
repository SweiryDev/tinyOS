#ifndef __SYSCALL
#define __SYSCALL

#include <process/task.h>

// System call numbers
typedef enum {
    SYS_YIELD = 0,  // simple syscall to yield the CPU
    SYS_WRITE_CONSOLE,
} syscall_number_t;

// Main system call handler
void syscall_handler(context_t* regs);

// Syscall wrapper 
void syscall(syscall_number_t syscall_num, ...);

// system call write console wrapper
void sys_write_console(const char* str);

#endif