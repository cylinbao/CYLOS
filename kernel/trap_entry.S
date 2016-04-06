#include <inc/mmu.h>
#include <inc/trap.h>

/* Use TRAPHANDLER_NOEC for traps where the CPU doesn't push an error code.
 * It pushes a 0 in place of the error code, so the trap frame has the same
 * format in either case.
 */
#define TRAPHANDLER_NOEC(name, num)					\
	.globl name;							\
	.type name, @function;						\
	.align 2;							\
	name:								\
	pushl $0;							\
	pushl $(num);							\
	jmp _alltraps

.text

/* TODO: Interface declaration for ISRs
 * Note: Use TRAPHANDLER_NOEC macro define other isr enrty
 *       The Trap number are declared in inc/trap.h which might come in handy
 *       when declaring interface for ISRs.
 */
TRAPHANDLER_NOEC(trap0, T_DIVIDE);
TRAPHANDLER_NOEC(trap1, T_DEBUG);
TRAPHANDLER_NOEC(trap2, T_NMI);
TRAPHANDLER_NOEC(trap3, T_BRKPT);
TRAPHANDLER_NOEC(trap4, T_OFLOW);
TRAPHANDLER_NOEC(trap5, T_BOUND);
TRAPHANDLER_NOEC(trap6, T_ILLOP);
TRAPHANDLER_NOEC(trap7, T_DEVICE);
TRAPHANDLER_NOEC(trap8, T_DBLFLT);
TRAPHANDLER_NOEC(trap10, T_TSS);
TRAPHANDLER_NOEC(trap11, T_SEGNP);
TRAPHANDLER_NOEC(trap12, T_STACK);
TRAPHANDLER_NOEC(trap13, T_GPFLT);
TRAPHANDLER_NOEC(trap14, T_PGFLT);
TRAPHANDLER_NOEC(trap16, T_FPERR);
TRAPHANDLER_NOEC(trap17, T_ALIGN);
TRAPHANDLER_NOEC(trap18, T_MCHK);
TRAPHANDLER_NOEC(trap19, T_SIMDERR);

TRAPHANDLER_NOEC(trap48, T_SYSCALL);
TRAPHANDLER_NOEC(trap500, T_DEFAULT);

TRAPHANDLER_NOEC(irq_timer, IRQ_OFFSET + IRQ_TIMER);
TRAPHANDLER_NOEC(irq_kbd, IRQ_OFFSET + IRQ_KBD);
TRAPHANDLER_NOEC(irq_serial, IRQ_OFFSET + IRQ_SERIAL);
TRAPHANDLER_NOEC(irq_spurious, IRQ_OFFSET + IRQ_SPURIOUS);
TRAPHANDLER_NOEC(irq_ide, IRQ_OFFSET + IRQ_IDE);
TRAPHANDLER_NOEC(irq_error, IRQ_OFFSET + IRQ_ERROR);

.globl default_trap_handler;
_alltraps:
  /* TODO: Since we are going to call default_trap_handler, we have to prepare
   *       the argument for it, which is placed on the stack.
   *       Thus push registers to fillup the Trapframe data structure.
   * 
   * Note: The x86 CPU will automatically save some register when interrupts
   *       occurs. Thus, you do not have to push those registers.
   *       Please reference lab3.docx for what registers are pushed by
   *       CPU.
   *       You may want to leverage the "pusha" instructions to reduce your work of
   *       pushing all the general purpose registers into the stack.
	 */
	  # Build trap frame.
  pushl %ds
  pushl %es                                        
  pushal

	pushl %esp # Pass a pointer which points to the Trapframe as an argument to default_trap_handler()
	call default_trap_handler
	popl %eax

  popal     
  popl %es
  popl %ds
	add $8, %esp # Cleans up the pushed error code and pushed ISR number
	iret # pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP!
