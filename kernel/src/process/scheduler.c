#include <process/scheduler.h>
#include <memory/vmm.h>
#include <utils/utils.h>

// Pointer to the currently running task
static task_t* current_task = 0;

// First task (kernel's main)
static task_t* kernel_task = 0;

void init_scheduler(){
    // Start with the kernel
    // for the first task
    kernel_task = (task_t*)vmm_alloc_page();
    memset(kernel_task, 0, sizeof(task_t));

    // circular list of one task
    kernel_task->next = kernel_task;

    // Set as the currently running task
    current_task = kernel_task;
}

void schedule(registers_task_t* regs){
    // Not ready yet
    if(!current_task) return;

    // Save the state of the current task
    current_task->regs = *regs;

    // Select the next task to run
    current_task = current_task->next;

    // Restore the state of the new task
    *regs = current_task->regs;
}
