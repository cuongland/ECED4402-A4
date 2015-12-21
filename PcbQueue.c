/*
 * PcbQueue.c
 *
 *  Created on: Nov 15, 2015
 *      Author: Duc Cuong Dinh
 */

#include <stdio.h>
#include "memory.h"
#include "Process.h"
#include "generalinfo.h"
#include "PcbQueue.h"
#include "PriorityQueue.h"
#include "cursor.h"
#include "uart.h"

//the global variable in the file PriorityQueue.c
extern volatile struct ManagementOfQueueElement *ManagementOfQueues;

//create an new PCB
//input: PID, priority level
//ouput: FALSE: if it is can not create a new PCB
//					reasons: unapprociated priority
//							 run out of memory
// get an memory block, and add it to tail of priority queue's level
unsigned int create_new_Pcb(unsigned int pid, unsigned int priority)
{
	volatile struct PCB *new_node;
	//checking priority
	if(priority>=MAX_PRIORITY_QUEUES_MANAGEMENT)
		return FALSE;
	//getting memory block
	new_node = (volatile struct PCB *) allocate(sizeof(struct PCB));
	printf("the address of node: %08x\n",(unsigned int)new_node);
	//check the address
	if(new_node==NULL)
		return FALSE;

	//assign the pID, priority, and StackFrame
	new_node->pid = pid;
	new_node->priority = priority;
	new_node->sp = NULL;

	//update the link
	//next must be NULL
	new_node->next = NULL;

	//the previous link is the end of ManagementOfQueues's level
	new_node->prev = (volatile struct PCB *)(ManagementOfQueues[priority].tail);

	//if this is the first node in the level, then the head is pointed to this one
	if(ManagementOfQueues[priority].head==NULL)
		ManagementOfQueues[priority].head = new_node;
	else //if the head is already pointed to somewhere else, the tail point to this
		ManagementOfQueues[priority].tail->next = new_node;

	//update the tail
	ManagementOfQueues[priority].tail = new_node;

	//new_node is set back to NULL before leaving
	new_node = (volatile struct PCB *)NULL;

	return TRUE;
}

//move the PCB to a new level
//the function is used to move the node to higher or lower level
//input: PCB, and new target level
//ouput: FALSE: if it is can not move PCB
//					reasons: target is wrong
//							 PCB does not exist
//							 PCB's priority is zero (the idle process)
// update the link and move to the new level
//not create a new PCB and copy data
unsigned int move_Pcb(unsigned int target,volatile struct PCB *current)
{
	//printf("<move_PCB: %d>\n",target);
	volatile struct PCB *temp_prev;
	volatile struct PCB *temp_next;

	// checking the conditions
	if(current==NULL || target>=MAX_PRIORITY_QUEUES_MANAGEMENT || target<1)
		return FALSE;

	//checking the priority of PCB
	if(current->priority==0)
			return FALSE;

	//getting the information
	temp_prev = current->prev;
	temp_next = current->next;

	//print out an address for debugs
	printf("(%08x %08x)", temp_prev,temp_next);

	//when temp_prev is NULL: it means the node is at the begin
	//			-> update the ManagementOfQueues's head
	//     temp_prev is not, it means the node is in the middle or in the end
	//			->update the previous node's next
	if(temp_prev!=NULL)
		temp_prev->next = temp_next;
	else
		ManagementOfQueues[current->priority].head = temp_next;

	//when temp_next is NULL: it means the node is at the end
	//			-> update the ManagementOfQueues's tail
	//     temp_next is not, it means the node is in the middle or in the begin
	//			->update the next node's prev
	if(temp_next!=NULL)
		temp_next->prev = temp_prev;
	else
		ManagementOfQueues[current->priority].tail = temp_prev;

	//at the moment, the current node is floating, there is no list is pointed to this node
	//the current node is move to the end of target level
	//update the link of the current node
	current->next = NULL;
	current->prev = ManagementOfQueues[target].tail;

	//if the priority level is empty, then the head must be NULL,
	//		->update head to point to this node
	//otherwise, the tail is update to point to this node
	if(ManagementOfQueues[target].head==NULL)
		ManagementOfQueues[target].head = current;
	else
		ManagementOfQueues[target].tail->next = current;

	//update the tail
	ManagementOfQueues[target].tail = current;

	//update the priority of the current node to match the level
	current->priority = target;
	printf("<move_PCB priority: %d - %d>\n",current->pid,current->priority);

	//done
	return TRUE;
}

//delete the PCB
//input: PCB
//ouput: FALSE: if it is can not move PCB
//					reasons: PCB does not exist
//							 PCB's priority is zero (the idle process)
// update the link and move to the new level
//not create a new PCB and copy data
unsigned int delete_Pcb(volatile struct PCB *current)
{
	volatile struct PCB *temp_prev;
	volatile struct PCB *temp_next;

	//checking the condition
	if(current==NULL || current->priority==0)
		return FALSE;

	printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>pid:%d>\n",current->pid);

	//getting an information
	temp_prev = current->prev;
	temp_next = current->next;

	//if the link of previous node is NULL
	//	-> this node is begin of the list,
	//		-> update the head of the queue
	//if the link is not NULL
	//	-> this node is in middle or in the end
	//		-> just update the previous node's next
	if(temp_prev==NULL)
		ManagementOfQueues[current->priority].head = temp_next;
	else
		temp_prev->next = temp_next;

	//if the link of next node is NULL
	//	-> this node is end of the list,
	//		-> update the tail of the queue
	//if the link is not NULL
	//	-> this node is in middle or in the begin
	//		-> just update the next node's prev
	if(temp_next==NULL)
	{
		ManagementOfQueues[current->priority].tail = temp_prev;
		ManagementOfQueues[current->priority].tail->next = temp_next;
	}
	else
		temp_next->prev = temp_prev;

	//update the current node
	//this point, the node is floating, there is no node is pointed to this
	current->priority = 0;
	current->next = NULL;
	current->prev = NULL;

	//release the memory of PCB and stackframe
	deallocate((unsigned int *) current->sp);
	deallocate((unsigned int *) current);

	//set these link to NULL
	current = (volatile struct PCB *) NULL;
	current->sp = NULL;

	//at this point, if process is assigned, fault will be occur
	//because PCB, SP, next, prev are all NULL
	//prioroty queue does not have any link being pointed to this
	return TRUE;
}

//convert number to string
//assume the number is 6 digits
void convert_to_string(volatile char *string, volatile unsigned int *location, unsigned int value)
{
	//assume pid is just up to 1 million
	unsigned int unit;
	unsigned int temp = value;
	for(unit=1000000;unit;unit = unit/10)
	{
		string[(*location)++] = (temp/unit) + '0';
		temp = temp % unit;
	}
}
