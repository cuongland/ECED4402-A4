///*
// * test_case.c
// *
// *  Created on: Nov 19, 2015
// *      Author: Duc Cuong Dinh
// */
//#include <stdio.h>
//
//#include "generalinfo.h"
//#include "Queue.h"
//#include "memory.h"
//#include "PriorityQueue.h"
//#include "uart.h"
//#include "systick.h"
//#include "PendSVHandler.h"
//#include "clock.h"
//#include "cursor.h"
//#include "Kernel.h"
//#include "ControlSoftware.h"
//#include "PcbQueue.h"
//#include "thread.h"
//
//extern volatile struct PCB *running;
//extern volatile struct ManagementOfQueueElement *ManagementOfQueues;
//
////adding the PCB to the system
//void testcase1(void)
//{
//	printf("testcase 1\n");
//	RegistrationProcess(printJ,1);
//	while(1);
//}
//
//void testcase2(void)
//{
//	printf("testcase 2\n");
//	RegistrationProcess(printJ,1);
//	printf_priority_queue();
//	while(1);
//}
//
//void testcase3(void)
//{
//	printf("testcase 3\n");
//	RegistrationProcess(printJ,1);
//	RegistrationProcess(printC,1);
//	printf_priority_queue();
//	while(1);
//}
//void testcase4(void)
//{
//	printf("testcase 4\n");
//	//create a queue
//	RegistrationProcess(printJ,1);
//	RegistrationProcess(printC,1);
//	printf_priority_queue();
//	delete_Pcb(ManagementOfQueues[1].head);
//	printf_priority_queue();
//	while(1);
//}
//void testcase5(void)
//{
//	printf("testcase 5\n");
//	//create a queue
//	RegistrationProcess(printJ,1);
//	RegistrationProcess(printC,1);
//	printf_priority_queue();
//	delete_Pcb(ManagementOfQueues[1].tail);
//	printf_priority_queue();
//	while(1);
//}
//void testcase6(void)
//{
//	struct PCB *temp;
//	printf("testcase 6\n");
//	//create a queue
//	RegistrationProcess(printJ,1);
//	RegistrationProcess(printC,1);
//	RegistrationProcess(printA,1);
//	printf_priority_queue();
//	temp = (struct PCB *)ManagementOfQueues[1].head;
//	temp = (struct PCB *)temp->next;
//	delete_Pcb(temp);
//	printf_priority_queue();
//	while(1);
//}
//
//void testcase7(void)
//{
//	//
//	printf("testcase 7\n");
//	//create a queue
//	//level 1
//	RegistrationProcess(printJ,1);
//	RegistrationProcess(printA,1);
//	//level 3
//	RegistrationProcess(printC,3);
//	printf_priority_queue();
//	move_Pcb(3,ManagementOfQueues[1].head);
//	printf_priority_queue();
//	while(1);
//}
//void testcase8(void)
//{
//		printf("testcase 8\n");
//		//create a queue
//		//level 1
//		RegistrationProcess(printJ,1);
//		RegistrationProcess(printA,1);
//		//level 3
//		RegistrationProcess(printC,3);
//		printf_priority_queue();
//		move_Pcb(1,ManagementOfQueues[3].head);
//		printf_priority_queue();
//		while(1);
//}
//
//void testcase9(void)
//{
//	struct PCB *temp;
//	printf("testcase 9\n");
//	//create a queue
//	//level 1
//	RegistrationProcess(printJ,1);
//	RegistrationProcess(printA,1);
//	RegistrationProcess(printC,1);
//	//level 3
//	//empty
//	temp = (struct PCB *)ManagementOfQueues[1].head;
//	temp = (struct PCB *)temp->next;
//	printf_priority_queue();
//	move_Pcb(3,temp);
//	printf_priority_queue();
//	while(1);
//}
//void testcase10(void)
//{
//	//level 1
//	RegistrationProcess(printJ,1);
//	RegistrationProcess(printA,1);
//	RegistrationProcess(printC,1);
//	//level 3
//	RegistrationProcess(printA,3);
//	//level 2
//	RegistrationProcess(printC,2);
//	printf_priority_queue();
//	set_running();
//	printf("the next process is: %d in the priority %d\n", running->pid, running->priority);
//	while(1);
//}
//
//void testcase11(void)
//{
//	RegistrationProcess(printJ,1);
//	RegistrationProcess(printA,1);
//	Swap(1);
//	printf_priority_queue();
//	printf("the current process is: %d in the priority %d\n", running->pid, running->priority);
//	Swap(1);
//	printf_priority_queue();
//	printf("the current process is: %d in the priority %d\n", running->pid, running->priority);
//	while(1);
//}
