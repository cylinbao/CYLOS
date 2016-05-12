#include <inc/stdio.h>
#include <inc/kbd.h>
#include <inc/shell.h>
#include <inc/x86.h>
#include <kernel/mem.h>
#include <kernel/trap.h>
#include <kernel/picirq.h>
#include <kernel/task.h>
#include <kernel/syscall.h>
#include <kernel/timer.h>

extern void init_video(void);
extern Task *cur_task;

void kernel_main(void)
{
	extern char stext[];
	extern char etext[], end[], data_start[],rdata_end[];
	extern void task_job();

	init_video();

	trap_init();
	pic_init();
	kbd_init();
  mem_init();

  printk("Kernel code base start=0x%08x to = 0x%08x\n", stext, etext);
  printk("Readonly data start=0x%08x to = 0x%08x\n", etext, rdata_end);
  printk("Kernel data base start=0x%08x to = 0x%08x\n", data_start, end);
  //timer_init();
  syscall_init();

  task_init();

  /* Enable interrupt */
  __asm __volatile("sti");

	/*
	uint32_t UTEXT_SZ;
	uint32_t UDATA_SZ;
	uint32_t UBSS_SZ;
	uint32_t URODATA_SZ;

	extern char UTEXT_start[], UTEXT_end[];
	extern char UDATA_start[], UDATA_end[];
	extern char UBSS_start[], UBSS_end[];
	extern char URODATA_start[], URODATA_end[];

  UTEXT_SZ = (uint32_t)(UTEXT_end - UTEXT_start);
  UDATA_SZ = (uint32_t)(UDATA_end - UDATA_start);
  UBSS_SZ = (uint32_t)(UBSS_end - UBSS_start);
  URODATA_SZ = (uint32_t)(URODATA_end - URODATA_start);

  setupvm(kern_pgdir, (uint32_t)UTEXT_start, UTEXT_SZ);
  setupvm(kern_pgdir, (uint32_t)UDATA_start, UDATA_SZ);
  setupvm(kern_pgdir, (uint32_t)UBSS_start, UBSS_SZ);
  setupvm(kern_pgdir, (uint32_t)URODATA_start, URODATA_SZ);

	test();

	shell();
	*/

  printk("after task_init\n");

  lcr3(PADDR(cur_task->pgdir));

  printk("pass loading pgdir\n");

  /* Move to user mode */
  asm volatile("movl %0,%%eax\n\t" \
  "pushl %1\n\t" \
  "pushl %%eax\n\t" \
  "pushfl\n\t" \
  "pushl %2\n\t" \
  "pushl %3\n\t" \
  "iret\n" \
  :: "m" (cur_task->tf.tf_esp), "i" (GD_UD | 0x03), "i" (GD_UT | 0x03), "m" (cur_task->tf.tf_eip)
  :"ax");
}
