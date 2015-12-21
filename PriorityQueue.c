/*
 * PriorityQueue.c
 *
 *  Created on: Nov 14, 2015
 *      Author: Duc Cuong Dinh
 */
#include <stdio.h>

#include "generalinfo.h"
#include "PriorityQueue.h"
#include "memory.h"
#include "Queue.h"
#include "Process.h"
#include "PcBQueue.h"

extern void terminate(void);

volatile struct ManagementOfQueueElement *ManagementOfQueues;
volatile unsigned int pID;
volatile struct PCB *running;

volatile struct currentRunningSate currentRunnig;

//initialization for Priority QUeue
//note: struct ManagementOfQueueElement has its size is
//      (4 bytes x2 variables) x 5 levels = 40 bytes
//      the smalles block or memory is 128, so that MAX_PRIORITY_SIZE is taken 128
//set pID is zero
//set running process is NULL
void InitializePriorityQueueManagement()
{
	unsigned int i;
	pID = 0;
	//getting memory for management of Queue
	ManagementOfQueues = (volatile struct ManagementOfQueueElement *)allocate(MAX_PRIORITY_SIZE);
	//setting each pointers to NULL
	for(i=0;i<MAX_PRIORITY_QUEUES_MANAGEMENT;i++)
	{
		ManagementOfQueues[i].head = NULL;
		ManagementOfQueues[i].tail = NULL;
	}
	//seting the running process to NULL
	running = NULL;
}

//return pID and update pID by one
unsigned int getPid()
{
  return (pID++);
}

//select the next process
//the function will scan the highest priority to ground
//if the current process is deleted, its priority is set zero
//scanning will be stop until it found the highest process to run
unsigned int set_running(void)
{
	unsigned int priority;
	int level;

	//checking the current running process
	//if it is not NULL, getting its priority
	//if it is NULL, set priroty to be zero
	if(running!=NULL)
		priority = running->priority;
	else
		priority = 0;

	//scanning priority, from highest priority to zero (4->0)
	//Note: level 0 is ilde process, there is always a process to ready run
	for(level = MAX_PRIORITY_QUEUES_MANAGEMENT-1; level>=0; level--)
	{
		//if head is pointed to some node, there is at least a process in its level
		if(ManagementOfQueues[level].head != NULL)
		{
			//if priority is the same level
			//it mean the current process is in this level,
			//set the next process
			//if the next is NULL, the end of the list, reset to the head
			if(priority == level)
			{
				//set to the next process
				running = running->next;
				//if it is NULL, getting the head of queue
				if(running==NULL)
					running = ManagementOfQueues[level].head;
			}
			else //setting the head for the next running
				running = ManagementOfQueues[level].head;
			return TRUE;
		}
	}

	return TRUE;
}

/*
 * create a new PCB by assigning function's pointer and priority
 */
int RegistrationProcess(function_pointer func, unsigned int priority)
{
	unsigned int baseAddress;

	//check if it is the valid priority level
	if(priority > MAX_PRIORITY_QUEUES_MANAGEMENT)
    {
    	 printf("Invalid priority\n");
    	 return REGISTRATION_FAILED;
    }

	//Step 1: allocate space for PCB
	if(create_new_Pcb(getPid(),priority))
		printf("add successfully\n");
	else
		printf("add fail\n");

	//Step 2: allocate space for stack
	baseAddress=(unsigned int)allocate(STACKSIZE);

	//check baseAddress
	if(baseAddress==NULL)
	{
		printf("Allocate stack fail\n");
		return REGISTRATION_FAILED;

	}

	//Step 3: set stack pointer
	ManagementOfQueues[priority].tail->sp = baseAddress + STACKSIZE - (unsigned int) sizeof(struct StackFrame);
	printf("the address of stackframe: %08x\n",(unsigned int)(ManagementOfQueues[priority].tail->sp));
	//step 4: set up stack frame
	struct StackFrame *stackFrame = (struct StackFrame *)(ManagementOfQueues[priority].tail->sp);

	//set these value for register for easier debugs
	//there is no need to set these values
	stackFrame->r0 = 0x00000000;
	stackFrame->r1 = 0x11111111;
	stackFrame->r2 = 0x22222222;
	stackFrame->r3 = 0x33333333;
	stackFrame->r4 = 0x44444444;
	stackFrame->r5 = 0xDEADBEEF;
	stackFrame->r6 = 0x66666666;
	stackFrame->r7 = 0x77777777;
	stackFrame->r8 = 0x88888888;
	stackFrame->r9 = 0x99999999;
	stackFrame->r10 = 0x10101010;
	stackFrame->r11 = 0x11011011;
	stackFrame->r12 = 0x12121212;

	//set the xpsr to put the system into thumb mode.
	//this value is a must to set
	stackFrame->xpsr = 0x01000000;

	//set the pc to point to the function it will be running
	//PC is what process will run
	//LR is when process is end, it will call terminate function in kernel to
	//	delete the PCB
	stackFrame->pc = (unsigned long)func;
	stackFrame->lr = (unsigned long)terminate;

	//update running
	if(running == NULL || priority > running->priority)
		running = ManagementOfQueues[priority].head;

	//return the process id when it finished creates the
	return (int)(ManagementOfQueues[priority].tail->pid);
}

//print out the the information in each level
//the function is used to checking Priority Queue
//it is used to check the link in each node
void printf_priority_queue(void)
{
	unsigned int i;
	volatile struct PCB *temp;
	printf("--------------------------------------------------\n");
	for(i=0; i<MAX_PRIORITY_QUEUES_MANAGEMENT;i++)
	{
		printf("level: %d\n",i);
		printf("(head: %08x) (tail: %08x)\n",ManagementOfQueues[i].head,ManagementOfQueues[i].tail);
		temp = ManagementOfQueues[i].head;
		while(temp)
		{
			printf("pid: %d - %d %08x %08x\n",temp->pid, temp->priority,temp->prev,temp->next);
			temp = temp->next;
		}
	}
	printf("--------------------------------------------------\n");
}
