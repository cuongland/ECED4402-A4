/*
 * PriorityQueues.h
 *
 *  Created on: 2015-11-12
 *      Author: Administrator
 */

#ifndef PRIORITYQUEUES_H_
#define PRIORITYQUEUES_H_

#include "generalinfo.h"
#include "Process.h"
/*
 * 4 levels of priority queues and 1 level for idle
 */
#define MAX_PRIORITY_QUEUES_MANAGEMENT 	 4
#define REGISTRATION_FAILED 			-1
#define MAX_PRIORITY_SIZE				 128

struct ManagementOfQueueElement
{
	volatile struct PCB *head;
	volatile struct PCB *tail;
};

struct currentRunningSate
{
	struct PCB *runningpcb;
	unsigned int currentRunningPriorityLevel;
};

#define REGISTRATION_FAILED -1
unsigned int set_running(void);
int RegistrationProcess(function_pointer func,unsigned int priority);

void InitializePriorityQueueManagement(void);
void printf_priority_queue(void);
void abc(void);
void abcd(void);
void abcde(void);
#endif /* PRIORITYQUEUES_H_ */
