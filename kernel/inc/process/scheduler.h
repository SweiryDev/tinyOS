// Round-Robin Scheduler
#ifndef __PROCESS_SCHEDULER
#define __PROCESS_SCHEDULER

#include <process/task.h>

// Initialize the scheduler, initiate kernel task 
// and prepare the systems for multitasking
void init_scheduler();

// Main scheduler function, called by the timer interrupt
// save the state of the current task and swith to the next 
// task in the list
void schedule(registers_task_t *regs);

#endif