/*
 * process.h
 *
 *  Created on: 2015-11-12
 *      Author: Administrator
 */

#ifndef PROCESS_H_
#define PROCESS_H_

#define MSP_RETURN 0xFFFFFFF9 	 //LR value: exception return using MSP as SP
#define PSP_RETURN 0xFFFFFFFD 	 //LR value: exception return using PSP as SP

/* Cortex default stack frame */
struct StackFrame{
/* Registers saved by s/w (explicit) */
/* There is no actual need to reserve space for R4-R11, other than
 * for initialization purposes.  Note that r0 is the h/w top-of-stack.
 */
	unsigned long r4;
	unsigned long r5;
	unsigned long r6;
	unsigned long r7;
	unsigned long r8;
	unsigned long r9;
	unsigned long r10;
	unsigned long r11;

	// Stacked by hardware/CPU (implicit)
	unsigned long r0;
	unsigned long r1;
	unsigned long r2;
	unsigned long r3;
	unsigned long r12;
    // What could be the use of LR register ?!
	unsigned long lr;

    // PC should hold the address of process entry point
	unsigned long pc;

    // Initialize to 0x010000000, why?
	unsigned long xpsr;
};

/*
 * Process control block
 * NOTE: this structure may require additional fields
 */
struct PCB{
	unsigned int pid;
	// Stack pointer - r13 (PSP)
	unsigned int sp;
	// Links to adjacent PCBs
	volatile struct PCB *next;
	volatile struct PCB *prev;

	//added
	unsigned int priority;
};

/*
 * Returns contents of PSP (current process stack)
 * Discussion: compiler uses R0 to return the value,
 */
unsigned long get_PSP(void);

/*
 * Returns contents of MSP (main stack)
 */
unsigned long get_MSP(void);


/*
 * set PSP to ProcessStack
 */
void set_PSP(volatile unsigned long ProcessStack);

/*
 * Set MSP to MainStack
 */
void set_MSP(volatile unsigned long MainStack);

/*
 * Save r4..r11 on process stack
 */

void save_registers();
/*
 * Restore r4..r11 from stack to CPU
 */
void restore_registers();

unsigned long get_SP();

/*
 * Assign 'data' to R7; since the first argument is R0, this is
 * simply a MOV for R0 to R7
 */
void setR7(volatile unsigned long value);

void assignR7(volatile unsigned long data);

unsigned int get_register_7(void);

#endif /*PROCESS_H_*/
