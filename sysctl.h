#ifndef SYSCTL_H_
#define SYSCTL_H_

/* 
 * file: "sysctl.h"
 *
 * ECED 4402
 * Author: Emad Khan 
 * 
 * This file contains the following:
 *
 * Structures for System Control of Run-Mode
 * Clock Configuration 1 and 2 registers (SYSCTL_RCGC1, SYSCTL_RCGC2 )
 * which are used to provide control for the system clock.
 *
 * In particular, SYSCTL_RCGC1 contains clock-feed control for UART0 
 * and SSI0 (SPI), while SYSCTL_RCGC2 contains clock-feed signal for
 * PORTS E and F.
 *
 *
 * Date of creation: April 2nd, 2012
 * Date last edited: March 14th, 2013
 * Version: 2
 *
 */

struct SYSCTL_RCGC1		// Run Mode Clock Gating Control Register 1 bits
{
	volatile unsigned UART0 : 1; 	// UART0 Clock Gating Control
	volatile unsigned UART1 : 1; 	// UART1 Clock Gating Control
	volatile unsigned UART2 : 1; 	// UART2 Clock Gating Control
	
	const unsigned RES3 : 1;			// Reserved
	
	volatile unsigned SSI0 : 1; 	// SSI0 Clock Gating Control
	volatile unsigned SSI1 : 1; 	// SSI1 Clock Gating Control
	
	const unsigned RES6_7 : 2;			// Reserved
	
	volatile unsigned QEI0 : 1; 	// QEI0 Clock Gating Control
	volatile unsigned QEI1 : 1; 	// QEI1 Clock Gating Control
	
	const unsigned RES10_11 : 2;		// Reserved
	
	volatile unsigned I2C0 : 1; 	// I2C0 Clock Gating Control
	
	const unsigned RES13 : 1;			// Reserved
	
	volatile unsigned I2C1 : 1; 	// I2C1 Clock Gating Control
	
	const unsigned RES15 : 1;			// Reserved
	
	volatile unsigned TIMER0 : 1; 	// TIMER0 Clock Gating Control
	volatile unsigned TIMER1 : 1; 	// TIMER1 Clock Gating Control
	volatile unsigned TIMER2 : 1; 	// TIMER2 Clock Gating Control	
	volatile unsigned TIMER3 : 1; 	// TIMER3 Clock Gating Control	
	
	const unsigned RES20_23 : 4;			// Reserved
	
	volatile unsigned COMP0 : 1;	// Analog Comparator 0 Clock Gating
	volatile unsigned COMP1 : 1;	// Analog Comparator 1 Clock Gating
	volatile unsigned COMP2 : 1;	// Analog Comparator 2 Clock Gating
	
	const unsigned RES27 : 1; 				// Reserved
	
	volatile unsigned I2S0 : 1; 	// I2S0 Clock Gating
	
	const unsigned RES29 : 1; 				// Reserved
	
	volatile unsigned EPI0: 1; 		// EPI0 Clock Gating
	
	const unsigned RES31 : 1; 				// Reserved
};

struct SYSCTL_RCGC2		// Run Mode Clock Gating Control Register 2 bits
{
	volatile unsigned GPIOA : 1; 	// PORT A Clock Gating Control
	volatile unsigned GPIOB : 1; 	// PORT B Clock Gating Control
	volatile unsigned GPIOC : 1; 	// PORT C Clock Gating Control
	volatile unsigned GPIOD : 1; 	// PORT D Clock Gating Control
	volatile unsigned GPIOE : 1; 	// PORT E Clock Gating Control
	volatile unsigned GPIOF : 1; 	// PORT F Clock Gating Control
	volatile unsigned GPIOG : 1; 	// PORT G Clock Gating Control
	volatile unsigned GPIOH : 1; 	// PORT H Clock Gating Control
	volatile unsigned GPIOJ : 1; 	// PORT J Clock Gating Control
	
	const unsigned RES9_12 : 4; 		// Reserved
	
	volatile unsigned UDMA : 1; 	// Micro-DMA Clock Gating Control
	
	const unsigned RES14_15 : 2; 		// Reserved
	
	volatile unsigned USB0 : 1; 	// USB0 Clock Gating Control
	
	const unsigned RES17_27 : 11;		// Reserved
	
	volatile unsigned EMAC0 : 1; 	// MAC0 Clock Gating Control
	
	const unsigned RES29 : 1;			// Reserved	
	
	volatile unsigned EPHY0 : 1; 	// PHY0 Clock Gating Control
	
	const unsigned RES31 : 1;			// Reserved	
};

extern struct SYSCTL_RCGC1 * const SYSCTL_RCGC1_R; 	// Run Mode Clock Gating Control Register 1
extern struct SYSCTL_RCGC2 * const SYSCTL_RCGC2_R; 	// Run Mode Clock Gating Control Register 2

#endif /*SYSCTL_H_*/


