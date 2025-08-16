#ifndef __PROCESS_TASK_H
#define __PROCESS_TASK_H

#include <types.h>

// This struct represents the FULL context saved by the CPU and our ISR stub.
// The order MUST match the order of PUSH operations.
typedef struct __attribute__((packed)) {
    // Pushed by our 'PUSHALL' macro
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
    uint64_t rdi, rsi, rbp, rdx, rcx, rbx, rax;

    // Pushed by our ISR stub
    uint64_t int_no, err_code;

    // Pushed by the CPU automatically on interrupt
    uint64_t rip, cs, rflags, rsp, ss;
} context_t;

// The main structure for a task
typedef struct task {
    context_t context;      // The saved context of the task
    struct task* next;      // Pointer to the next task in the list
} task_t;

#endif