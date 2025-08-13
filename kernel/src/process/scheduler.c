#include <process/scheduler.h>
#include <memory/vmm.h>
#include <utils/utils.h>

static task_t* current_task = 0;
static task_t* kernel_task = 0;

void init_scheduler() {
    kernel_task = (task_t*)vmm_alloc_page();
    memset(kernel_task, 0, sizeof(task_t));
    kernel_task->next = kernel_task;
    current_task = kernel_task;
}

// This is the corrected schedule function
void schedule(context_t* context) {
    if (current_task == 0) return;

    // 1. Save the full context of the current task
    memcpy(&current_task->context, context, sizeof(context_t));

    // 2. Select the next task
    current_task = current_task->next;

    // 3. Restore the full context of the new task
    memcpy(context, &current_task->context, sizeof(context_t));
}

// This is the corrected create_task function
void create_task(void (*start_address)()) {
    task_t* new_task = (task_t*)vmm_alloc_page();
    memset(new_task, 0, sizeof(task_t));

    void* stack = vmm_alloc_page();

    // Set the initial state for the new task's context
    new_task->context.rip = (uint64_t)start_address;
    new_task->context.rsp = (uint64_t)stack + 4096;
    new_task->context.rflags = 0x202; // Enable interrupts
    new_task->context.cs = 0x08;      // Kernel Code Segment
    new_task->context.ss = 0x10;      // Kernel Data/Stack Segment

    // Add the new task to the list
    new_task->next = kernel_task->next;
    kernel_task->next = new_task;
}