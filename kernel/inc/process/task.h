#ifndef __PROCESS_TASK
#define __PROCESS_TASK

#include <types.h>

// State structure for general purpose cpu registers
typedef struct {
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
    uint64_t rbp, rdi, rsi, rdx, rcx, rbx, rax;
    uint64_t rip;       // instruction pointer
    uint64_t cs;        // code segment
    uint64_t rflags;    // CPU flags
    uint64_t rsp;       // stack pointer
    uint64_t ss;        // stack segment 
} registers_task_t;

// Task (or process) main structure
// Linked list
typedef struct task {
    registers_task_t regs;   // saved registers
    struct task* next;  // pointer to the next task in the list for the scheduler
} task_t;

#endif