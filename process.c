/*
 * Process.c
 * Support functions
*/
#include <stdio.h>
#include "process.h"
#include "Kernel.h"

extern volatile struct PCB *running;
extern void set_running(void);

unsigned long get_PSP(void)
{
/* Returns contents of PSP (current process stack */
__asm("	mrs r0, psp");
__asm("	bx lr");
return 0;
}

unsigned long get_MSP(void)
{
/* Returns contents of MSP (main stack) */
__asm("	mrs r0, msp");
__asm("	bx lr");
return(0);
}

void set_PSP(volatile unsigned long ProcessStack)
{
/* set PSP to ProcessStack */
__asm("	msr	psp, r0");
}

void set_MSP(volatile unsigned long MainStack)
{
/* Set MSP to MainStack */
__asm("	msr	msp, r0");
}

void save_registers()
{
/* Save r4..r11 on process stack */
__asm("	mrs r0,psp");
__asm("	stmdb r0!,{r4-r11}");	/* Store multiple, decrement before */
__asm("	msr	psp,r0");
}

void restore_registers()
{
/* Restore r4..r11 from stack to CPU */
__asm("	mrs	r0,psp");
__asm("	ldmia r0!,{r4-r11}");	/* Load multiple, increment after */
__asm("	msr	psp,r0");
}

unsigned long get_SP()
{
__asm(" 	mov r0,SP"); /**** Leading space required -- for label ****/
__asm("		bx lr");
return 0;
}

void assignR7(volatile unsigned long data)
{
/* Assign 'data' to R7; since the first argument is R0, this is
 * simply a MOV for R0 to R7
 */
 __asm("	mov r7,r0");

}

//getting a space for returning value
//it is stored in register 7
unsigned int get_register_7(void)
{
	//getting PSP
	struct StackFrame *sp = (struct StackFrame *)get_PSP();
	//KernelCall is trying to get R7 from SP
	struct kcallargs *kernel_call_args = (struct kcallargs *) sp -> r7;
	return (unsigned int)(kernel_call_args->code);
}
