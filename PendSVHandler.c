/*
 * PendSVHandler.c
 *
 *  Created on: Nov 15, 2015
 *      Author: Duc Cuong Dinh
 */

#include <stdio.h>

#include "Process.h"
#include "PriorityQueue.h"
#include "Kernel.h"
#include "Queue.h"

//the global variable in PriorityQueue
extern volatile struct PCB *running;

//swap process
//there is no input for this function
//output: always FALSE
//put back data in register to stackframe and loading the next process to registers
unsigned int Swap(unsigned int data)
{
	//get back PSP

	//store data back to stackframe
	running->sp = (unsigned int) get_PSP();

	//getting the next process
	set_running();

	//load PSP
	set_PSP(running->sp);

	return FALSE;
}

//PendSV Handler
//no input
//no output
// when PendSV is called, it will check the Queue
// it will get the task from the queue and process it,
//	-> the task can be transmitting string to uart
// 					   swaping the process
//					   ...
//it will back back where it is from when no task in the queue
void PendSVHandler(void)
{
	//printf("in pendsv\n");
	static unsigned int status;
	unsigned int data;
	function_pointer func_;
	//initial value
	status = TRUE;

	//save the register before process
	//save_registers();

	//staying in the queue until all tasks are completed
	while(status)
	{

		//trying to load the task
		//	-> if the task is needed more time, it will wait until the task is completed
		//		 example: sending string to uarts, it needs some time to transmit
		//	other task so far does not need to wait
		if(dequeue(&data,&func_))
			while(func_(data));
		else // end the queue
			status = FALSE;
	}

	//load the value back from stack
	//restore_registers();
	//printf("out pendsv\n");
}
