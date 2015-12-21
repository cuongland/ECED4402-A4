/*
 * ACriticalRegion.h
 *
 *  Created on: Oct 14, 2015
 *      Author: Duc Cuong Dinh
 */

#ifndef ACRITICALREGION_H_
#define ACRITICALREGION_H_

#include "generalinfo.h"

#define MAXINUMQUEUES	32
#define STATUS_BIT		31
#define MASK_STATUS		0x7FFFFFFF

//structure of QueueElement
//there are 32 bits for data and 32 bits address for function
//in data section: the bit 31th is used for status
//                 the bit 0->30 is used for storing data
//in address function: the address has a range from	0x0 -> 0x1FFFFFFF
//					  (if data is needed, the status can be moved to address)
//					  because the most three bits are unused
struct QueueElement
{
	unsigned int data;
	function_pointer function;
};

//dequeue:
//input: pointer to get values for data and function's address
//output: TRUE: load the data from the queue successfully
//		  FALSE: nothings in the queue to load, queue is empty
unsigned int dequeue(unsigned int *data, function_pointer *func);

//enqueue:
//input: data(max size : 31 bits) and address of function
//output: TRUE: put the data to the queue successful (having a space to store)
//		  FALSE: can not store the data into the nodes because of queue is full
unsigned int enqueue(unsigned int data, function_pointer func);

//initialization of acritical Region
//there is get dynamic memory and clear data for data section
//(4 bytes for data, and 4 bytes for function address) x 16 slots = 128 bytes
// it is exactly for a space 1 block of memory in small chunks
void initialize_Queue(void);

//for testing
//showing data in each nodes
void printing_queue(void);

#endif /* ACRITICALREGION_H_ */
