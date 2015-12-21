/*
 * systick.c
 *
 *  Created on: Oct 14, 2015
 *      Author: Duc Cuong Dinh
 */
#include <stdio.h>
#include <stdlib.h>

#include "systick.h"
#include "generalinfo.h"
#include "PendSVHandler.h"
#include "Queue.h"

//#define testing
extern unsigned int update_clock(unsigned int data);
extern unsigned int send_clock_to_uart(unsigned int data);
extern unsigned int state_stopwatch;
extern unsigned int print_terminal(unsigned int data);
//initialization of Systick
//input: 0->16777216 for RELOAD register
//no output
void SysTickInit(unsigned int Period)
{
	// Must be between 0 and 16777216
	SYSTICK_RELOAD_VALUE = Period - 1;
	// Set the interrupt bit in STCTRL
	// Set the clock source to internal and enable the counter
	SYSTICK_CONTROL_N_STATUS|= SYSTICK_INTERRUPT_FLAG|SYSTICK_CLOCK_SOURCE_FLAG;
	SYSTICK_CONTROL_N_STATUS|= SYSTICK_ENABLE_FLAG;
}
extern void printf_priority_queue(void);
//interrupt event
void SysTickHandler(void)
{
#ifdef testing
	printf("SysTick\n");
#endif

	//diable global interrupt
	//DISABLE_GLOBAL_INTERRUPT();

	//added in here

	//adding swap process to queue
	enqueue(0,Swap);

	//call PendSV handler to process
	NVIC_INT_CTRL_R |= TRIGGER_PENDSV;

}
