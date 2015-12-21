/*
 * semaphore.c
 *
 *  Created on: Dec 6, 2015
 *      Author: Duc Cuong Dinh
 */

/*
 * semaphore.c
 */

#include "semaphore.h"

void semaWait(unsigned int *semaphore){
	/*
	 * Implementation of a busy-wait:
	 * - Atomic test the semaphore for OPEN state
	 *  - If OPEN, CLOSE semaphore
	 *  - If CLOSED, repeat test until OPEN state detected (and CLOSE semaphore)
	 * This is an example of the the ARM's version of an atomic instruction
	 * R0 contains the address of semaphore
	 */
	asm("	MOV  	R3,#0x1");	/* R3 = CLOSED */
	/* Busy-wait loop */
	asm("BusyWait:");
	/*
	 * Load R1 contents of semaphore (location R0 + 0 or the 'semaphore')
	 * This is an EXclusive load, so tag memory location 'semaphore'
	 */
	asm("	LDREX	R1,[R0,#0]");
	/*
	 * Test R1 (value of 'semaphore') for CLOSED (0x01)
	 */
	asm("	CMP  	R1,#0x1");	/* Test R1 for CLOSED */
	/*
	 * If-Then (IT) instruction:
	 * Allows up to four conditional instructions to be used
	 * This *must* be used with conditional instructions when machine is
	 * running Thumb mode; up to four conditional instructions allowed (32-bit
	 * architecture has condition-codes built into instructions; not available
	 * in Thumb mode, hence IT instruction)
	 * Format: ITxyz cond
	 * COND - condition for first instruction execution
	 * X - Then/Else for second instruction
	 * Y - T/E for third instruction
	 * Z - T/E for fourth instruction
	 */
	asm("	ITT  	NE");   	/* If-Then first inst and second inst if CMP is NE */
	/*
	 * The following two instructions are executed only if CMP failed (i.e., NE)
	 *
	 * Store R3 (CLOSED) into the semaphore location [R0 + 0]
	 * If no other access of semaphore has taken place (i.e., tag unchanged),
	 * result is 0 otherwise 1; result is written to R2
	 * Note conditional execution: CMP R1 (above) with CLOSED must have failed
	 * meaning the 'semaphore' was OPEN; this results in the NE result
	 */
	asm("	STREXNE	R2,R3,[R0,#0]");
	/*
	 * Was writing R3 (CLOSED) to 'semaphore' successful?
	 * Compare R2 (result of STREXNE) with #1 (failed write)
	 */
	asm("	CMPNE	R2,#1");
	/*
	 * R2 = 0 - store successful; R2 = 1 - store unsuccessful
	 *
	 * Branch if result of comparison indicates CMPNE indicates R2 equals 1
	 * Note coding trick:
	 * - If first comparison (R1,#1) indicated the semaphore was CLOSED, the result
	 * was EQ and the two conditional-execution instructions were not executed,
	 * meaning that control passed back to BusyWait
	 * - If first comparison (R1,#1)indicated the semaphore was OPEN, the result
	 * was NE, meaning that the two conditional-execution instructions were
	 * executed.  If the store failed, R2 would have a value of #1,
	 * meaning the comparison would be EQ, causing the BEQ to pass control
	 * to BusyWait.  However, if store was successful, R2 would have the
	 * value #0, meaning that the comparison would be NE, causing BEQ to
	 * fail.
	 */
	asm("	BEQ   	BusyWait");
	/*
	 * Reach this point only if:
	 * 1. Semaphore was OPEN
	 * 2. No other processes had accessed the semaphore
	 * 3. Semaphore was successfully CLOSED
	 * Otherwise waste CPU cycles with the busy-wait
	 */



}

void semaPost(unsigned int *semaphore){
	/*
	 * Clear the semaphore (i.e., OPEN it)
	 * R0 contains the address of the semphore in question
	 */
	*semaphore = SEMA_OPEN;
}


