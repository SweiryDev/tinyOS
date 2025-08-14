#include <process/scheduler.h>
#include <memory/vmm.h>
#include <utils/utils.h>
#include <driver/vga.h>

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

    context_t* initial_context = (context_t*)((uint64_t)stack + 4096 - sizeof(context_t));
    memset(&initial_context->rax, 0, sizeof(uint64_t) * 15);

    // Set the initial state for the new task's context
    initial_context->rip = (uint64_t)start_address;
    initial_context->rflags = 0x202; // Enable interrupts
    initial_context->cs = 0x08;      // Kernel Code Segment
    initial_context->ss = 0x10;      // Kernel Data/Stack Segment

    initial_context->rsp = (uint64_t)initial_context;
    memcpy(&new_task->context, initial_context, sizeof(context_t));

    // Add the new task to the list
    new_task->next = kernel_task->next;
    kernel_task->next = new_task;
}