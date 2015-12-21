/*
 * PendSVHandler.h
 *
 *  Created on: Nov 15, 2015
 *      Author: Duc Cuong Dinh
 */

#ifndef PENDSVHANDLER_H_
#define PENDSVHANDLER_H_

#define NVIC_INT_CTRL_R   (*((volatile unsigned long *) 0xE000ED04))
#define TRIGGER_PENDSV    0x10000000

#define NVIC_SYS_PRI3_R				(*((volatile unsigned long *) 0xE000ED20))
#define PENDSV_LOWEST_PRIORITY		0x00E00000

void PendSVHandler(void);
unsigned int Swap(unsigned int data);
#endif /* PENDSVHANDLER_H_ */
