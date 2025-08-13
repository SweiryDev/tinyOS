#ifndef __PROCESS_SCHEDULER_H
#define __PROCESS_SCHEDULER_H

#include <process/task.h>

void init_scheduler();

// The schedule function takes a pointer to the full context on the stack
void schedule(context_t* context);

void create_task(void (*start_address)());

#endif