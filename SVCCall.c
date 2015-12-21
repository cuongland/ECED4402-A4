/*
 * Kernel code: Supervisor call (SVC) handler example
 * ECED 4402
 *
 * 23 Oct 2014 - Fix SVC call (again?) to test bit 3 in SVCall()
 * 11 Mar 2013 - Handle passing argument to kernel by register
 * 05 Mar 2013 - Fix first SVC call to access PSP correctly
 *             - Updated comments
 * 20 Feb 2012 - First version
 */
#include <stdio.h>
#include "Process.h"
#include "Kernel.h"
#include "generalinfo.h"
#include "PriorityQueue.h"
#include "PcbQueue.h"
#include "PendSVHandler.h"
#include "Queue.h"

extern void SysTickInit(unsigned int Period);

extern volatile struct PCB *running;

//the function does not changed
void SVCall(void)
{
	// Save LR for return on the Main Stack
		__asm(" 	PUSH 	{LR}");

		/* Trapping source: MSP or PSP? */
		__asm(" 	TST 	LR, #4");	/* Bit 2: MSP (0) or PSP (1) */
		__asm(" 	BNE 	RtnViaPSP");	/* EQ zero - MSP; NE zero - PSP */

		/* Trapping source is MSP - save r4-r11 on stack (default, so just push) */
		__asm(" 	PUSH 	{r4-r11}");
		__asm(" 	MRS	r0,msp");
		__asm(" 	BL	SVCHandler");	/* r0 is MSP */
		__asm(" 	POP	{r4-r11}");
		__asm(" 	POP 	{PC}");

		/* Trapping source is PSP - save r4-r11 on psp stack (MSP is active stack) */
		__asm("RtnViaPSP:");
		__asm(" 	mrs 	r0,psp");
		__asm("		stmdb 	r0!,{r4-r11}");	/* Store multiple, decrement before */
		__asm("		msr	psp,r0");
		__asm(" 	BL	SVCHandler");	/* r0 Is PSP */

		/* Restore r4..r11 from trapping process stack  */
		__asm(" 	mrs 	r0,psp");
		__asm("		ldmia 	r0!,{r4-r11}");	/* Load multiple, increment after */
		__asm("		msr	psp,r0");
		__asm(" 	POP 	{PC}");

}

//the function does change
//in the first time: setup for SysTick
//later:
void SVCHandler(struct StackFrame *argptr)
{

	static int firstSVCcall = TRUE;
	struct kcallargs *kcaptur;

	if (firstSVCcall)
	{
		set_PSP(running -> sp + 32);

		firstSVCcall = FALSE;

		/* Start SysTick */
		SysTickInit(SYSTICK_TIME);

		__asm("	movw 	LR,#0xFFFD");  /* Lower 16 [and clear top 16] */
		__asm("	movt 	LR,#0xFFFF");  /* Upper 16 only */
		__asm("	bx 	LR");          /* Force return to PSP */

	}
	else /* Subsequent SVCs */
	{
		//retreat the register 7 back to kernel call struction
		kcaptur = (struct kcallargs *) argptr -> r7;
		//process the command
		//and return the pID
		//
		switch(kcaptur->code)
		{
			case GETID:
				//getting an id
				kcaptur->rtnvalue = running->pid;
				//return right away
				break;
			case PROMOTEBY:
				//the current priority is highest
				//move up the pcb-> the new highest priority is the current pcb running
				//there is just one pcb at this moment
				move_Pcb((running->priority + kcaptur->arg1),running);
				kcaptur->rtnvalue = running->pid;
				break;
			case DEMOTEBY:
				//the current priority is highest
				//move down the pcb
				//the highest priority is still remain the current level
				//if there are pcb in the same priority
				//otherwise, the highest will be the current one
				// let swap process to determine which level is highest
				//affect the priority queue right aways
				move_Pcb((running->priority - kcaptur->arg1),running);
				kcaptur->rtnvalue = running->pid;
				enqueue(0,Swap);
				break;
			case TERMINATE:

				kcaptur->rtnvalue = running->pid;
				delete_Pcb(running);
				enqueue(0,Swap);
				break;
			default:
				break;
		}
	}
}
