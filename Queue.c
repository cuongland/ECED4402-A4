/*
 * ACriticalRegion.c
 *
 *  Created on: Oct 14, 2015
 *      Author: Duc Cuong Dinh
 */
#include <stdio.h>

#include "generalinfo.h"
#include "memory.h"
#include "Queue.h"

struct QueueElement *array;
unsigned int head, tail;

//initialization of acritical Region
//there is get dynamic memory and clear data for data section
//(4 bytes for data, and 4 bytes for function address) x 16 slots = 128 bytes
// it is exactly for a space 1 block of memory in small chunks
void initialize_Queue(void)
{
	head = tail = 0;
	//(4 bytes+ 4byte) x 16 slot = 128 bytes
	array = (struct QueueElement *) allocate(LVL64);
	unsigned int i;
	//if the data is not clear, when the program running, the junk data can
	//affect the final result
	for(i=0;i<MAXINUMQUEUES;i++)
		array[i].data = 0;
}

//for testing
//showing data in each nodes
void printing_queue(void)
{
	unsigned int i;
	for(i=0;i<MAXINUMQUEUES;i++)
		printf("(%08x %08x)\n",array[i].data, array[i].function);
	printf("\n");
}

//enqueue:
//input: data(max size : 31 bits) and address of function
//output: TRUE: put the data to the queue successful (having a space to store)
//		  FALSE: can not store the data into the nodes because of queue is full
unsigned int enqueue(unsigned int data, function_pointer func)
{
	//diable global interrupt
	DISABLE_GLOBAL_INTERRUPT();
	unsigned int temp_head = head;
	//getting bit status
	unsigned int *status = BITBAND_ALIAS_ADDRESS((&array[temp_head].data),
																	STATUS_BIT);
#ifdef DEBUG_ENQUEUE
	printf("enqueue: <head:%d> <tail:%d> <status:%d> <status address:%08x> <data:%08x> <data address:%08x>\n", temp_head, tail, *status, status, array[temp_head].data, &array[temp_head].data);
#endif
	//if the status is 1/TRUE
	if(*status)
		//the queue is still full, can not added any extra
		return FALSE;

	//if the status is 0/ FALSE
	//the queue is still having some slots to store values
	//storing values
	array[temp_head].data = data;
	array[temp_head].function = func;
	//update the cursors/head
	temp_head++;
	temp_head = temp_head % MAXINUMQUEUES;
	//final: update status for nodes to set to TRUE at the most significant bit
	*status = 1;
	//update head
	head = temp_head;
	//enable the global interrup
	ENABLE_GLOBAL_INTERRUPT();
	return TRUE;
}

//dequeue:
//input: pointer to get values for data and function's address
//output: TRUE: load the data from the queue successfully
//		  FALSE: nothings in the queue to load, queue is empty
unsigned int dequeue(unsigned int *data, function_pointer *func)
{
	unsigned int temp_tail = tail;
	//getting bit status
	unsigned int *status = BITBAND_ALIAS_ADDRESS((&array[temp_tail].data),
																	STATUS_BIT);
	//if the status is 1/TRUE
	if(*status)
	{
		//having values to load
		//loading value
		*data = array[temp_tail].data & MASK_STATUS;
		*func = array[temp_tail].function;
		//updating the cursors/tail
		temp_tail++;
		temp_tail = temp_tail % MAXINUMQUEUES;
		//final: update the tail
		tail = temp_tail;
		//update the status for current nodes
		*status = 0;
		//return successful
		return TRUE;
	}
	//nothing to load, return false
	return FALSE;
}
