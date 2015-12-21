/*
 * systick.h
 *
 *  Created on: Oct 14, 2015
 *      Author: Duc Cuong Dinh
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_

#define SYSTICK_CONTROL_N_STATUS    (*((volatile unsigned int *)0xE000E010))
#define SYSTICK_RELOAD_VALUE 		(*((volatile unsigned int *)0xE000E014))
#define SYSTICK_CLOCK_SOURCE_FLAG   0x00000004
#define SYSTICK_INTERRUPT_FLAG      0x00000002
#define SYSTICK_ENABLE_FLAG     	0x00000001

//initialization of Systick
//input: 0->16777216 for RELOAD register
//no output
void SysTickInit(unsigned int Period);
//interrupt event
void SysTickStart(void);

#endif /* SYSTICK_H_ */
