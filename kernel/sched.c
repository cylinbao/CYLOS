#include <kernel/task.h>
#include <inc/x86.h>

#define ctx_switch(ts) \
  do { env_pop_tf(&((ts)->tf)); } while(0)

extern Task tasks[];
extern Task *cur_task;

/* This function will not return */
void run_task(Task *task)
{
	/* Update the state for the current task */
	if(cur_task->state == TASK_RUNNING)
		cur_task->state = TASK_RUNNABLE;

	/* Make current task point to picked task */
	cur_task = task;
	cur_task->remind_ticks = TIME_QUANT;
	cur_task->state = TASK_RUNNING;
	/* load the page directory for picked task */
	lcr3(PADDR(cur_task->pgdir));
	/* do the content switch */
	ctx_switch(cur_task);
}

/* TODO: Lab5
* Implement a simple round-robin scheduler (Start with the next one)
*
* 1. You have to remember the task you picked last time.
*
* 2. If the next task is in TASK_RUNNABLE state, choose
*    it.
*
* 3. After your choice set cur_task to the picked task
*    and set its state, remind_ticks, and change page
*    directory to its pgdir.
*
* 4. CONTEXT SWITCH, leverage the macro ctx_switch(ts)
*    Please make sure you understand the mechanism.
*/
void sched_yield(void)
{
	int i, idx;

	/* start from the next task */
	if(cur_task != NULL){
		idx = cur_task->task_id + 1;

		for(i = 0; i < NR_TASKS; i++){
			/* if the next task is runnable, then switch to it */
			if(tasks[idx].state == TASK_RUNNABLE)
				run_task(&tasks[idx]);
			idx = (idx + 1) % NR_TASKS;
		}
		
		/* if no task is runnable and current task is still running 
		 * then keep running the current task */
		if(cur_task->state == TASK_RUNNING)
			run_task(cur_task);
	}
	else
		panic("no task to be scheduled\n");
}
