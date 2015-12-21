/*
 * PcbQueue.h
 *
 *  Created on: Nov 15, 2015
 *      Author: Duc Cuong Dinh
 */

#ifndef PCBQUEUE_H_
#define PCBQUEUE_H_

#include "PriorityQueue.h"
#include "Process.h"

unsigned int create_new_Pcb(unsigned int pid, unsigned int priority);

unsigned int move_Pcb(unsigned int target,volatile struct PCB *current);


unsigned int delete_Pcb(volatile struct PCB *current);

unsigned int print_terminal(unsigned int data);

void convert_to_string(volatile char *string, volatile unsigned int *location,
															unsigned int value);
#endif /* PCBQUEUE_H_ */
