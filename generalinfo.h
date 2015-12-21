/*	FILE: generalinfo.h
 * 	the file is stored everything general information or macro functions
 *
 * 	Date: September 20, 2015
 * 	Authors: Duc Cuong Dinh (B00516935)
 * 	         Jie Zhang      (B00583787)
 *
 */

#ifndef GENERALINFO_H_
#define GENERALINFO_H_
/*
 *  adding library
 */


/*
 *  constant values
 */
#define NUL					0x0
#define TRUE    			1
#define FALSE   			0
#define ESC					0x1B
#define BASE_NUMBER_10		10
#define STACKSIZE			1024
#define NULL_INDEX			0xFF
#define SYSTICK_TIME		1600000
#define STRING_SIZE			128
#define ENTER_ROW			23

typedef unsigned int (*function_pointer)(unsigned int data);


#define BITBAND_ALIAS_BASE   0x22000000
#define BITBAND_ALIAS_ADDRESS(x, y) ((unsigned int *) ( (((unsigned int)(x))<<5) + ((y)<<2) + BITBAND_ALIAS_BASE))

//ASSEMBLY INSTRUCTION
#define ENABLE_GLOBAL_INTERRUPT() 	__asm(" cpsie	i");
#define DISABLE_GLOBAL_INTERRUPT() 	__asm(" cpsid	i");
#define SVC()		                __asm(" SVC    #0");

#define Configuration_and_Control	(*((volatile unsigned long *)0xE000ED14))

#define BASETHR						0x1


#define DIRE			1
#define SPEEDY			2
#endif /* GENERALINFO_H_ */
