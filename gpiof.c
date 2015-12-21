#include "gpiof.h"
#include "sysctl.h"
#include "Queue.h"
#include "messageatmel.h"
#include "HallSensor.h"
#include "PendSVHandler.h"

extern void IntEnable(unsigned long InterruptIndex);
/* 
 * file: "gpiof.c"
 *
 * ECED 4402
 * Author: Emad Khan 
 * 
 * This file contains the following:
 *
 * - Structure addresses for the various PortF registers.
 *
 * - PORTF_Init(void) - a function called to setup/initialize the detection of rising-edge
 *  interrupts on PortF bits 0 and 1. 
 * NOTE: Interrupts on PortF must be enabled seperately through the EN0 Register.
 *
 * - GPIOPortF_IntHandler(void) - the interrupt handler for PortF rising-edge interrupts.
 * NOTE: This function must be added to the interrupt vector table in "startup_ccs.c"
 *
 * Copy this file into your CCS project for ease in porting the rest of the PortF code
 * 
 * Date of creation: April 2nd, 2012
 * Date last edited: March 14th, 2013
 * Version: 2
 *
 */

// Structures associated with General Purpose Input/Output PORT F

struct PORTF_GPIODR * const PORTF_GPIODR_R = (struct PORTF_GPIODR *) 0x400253FC;	// GPIO Data Register
struct PORTF_GPIODIR * const PORTF_GPIODIR_R = (struct PORTF_GPIODIR *) 0x40025400;	// GPIO Direction Register

// Interrupt Sense Registers
struct PORTF_GPIOIS * const PORTF_GPIOIS_R = (struct PORTF_GPIOIS *) 0x40025404;	// GPIO Interrupt Sense Register
struct PORTF_GPIOIBE * const PORTF_GPIOIBE_R = (struct PORTF_GPIOIBE *)  0x40025408;	// GPIO Interrupt Both Edges Register
struct PORTF_GPIOIEV * const PORTF_GPIOIEV_R = (struct PORTF_GPIOIEV *) 0x4002540C;	// GPIO Interrupt Event Register
struct PORTF_GPIOIM * const PORTF_GPIOIM_R = (struct PORTF_GPIOIM *) 0x40025410;	// GPIO Interrupt Mask Enable
struct PORTF_GPIOICR * const PORTF_GPIOICR_R = (struct PORTF_GPIOICR *) 0x4002541C;	// GPIO Interrupt Clear Register

struct PORTF_GPIOAFSEL * const PORTF_GPIOAFSEL_R = (struct PORTF_GPIOAFSEL *) 0x40025420;	// GPIO Alternate 
struct PORTF_GPIODEN * const PORTF_GPIODEN_R = (struct PORTF_GPIODEN *) 0x4002551C;		// GPIO Digital Enable 
struct PORTF_GPIOPCTL * const PORTF_GPIOPCTL_R = (struct PORTF_GPIOPCTL *) 0x4002552C;		// GPIO Port Control 

void PORTF_Init(void)
{
	// Activate Port F
	SYSCTL_RCGC2_R -> GPIOF = ACTIVATE;

	// Make PF0 and PF1 Inputs
	PORTF_GPIODIR_R -> DIR	&= ~(1<<BIT0);
	PORTF_GPIODIR_R -> DIR	&= ~(1<<BIT1);

	// Enable Digital I/O
	PORTF_GPIODEN_R -> DEN |= (1<<BIT0) | (1<<BIT1);

	// Edge Sensitive Interrupt
	PORTF_GPIOIS_R -> IS &= ~(1<<BIT0);
	PORTF_GPIOIS_R -> IS &= ~(1<<BIT1);

	// Rising Edge Sensitive
	PORTF_GPIOIEV_R -> IEV |= (1<<BIT0) | (1<<BIT1);


	// Both Edge Sensitive
	//PORTF_GPIOIBE_R -> IBE |= (1<<BIT0) | (1<<BIT1);

	// Clear Pin Interrupts
	PORTF_GPIOICR_R -> IC |= (1<<BIT0) | (1<<BIT1);
		
	// Enable Pin Interrupts
	PORTF_GPIOIM_R -> IME |= (1<<BIT0) | (1<<BIT1);
	
	//Enable interrupt for Port F
	//Arg 30 for Port F
	IntEnable(30);
}	

// Must be added to startup_ccs.c's interrupt vector table

void GPIOPortF_IntHandler(void) 	
{
// Configured for Rising Edge triggered interrupts on Pins 0 and 1

// Clear the two interrupts
PORTF_GPIOICR_R -> IC |= (1<<BIT0) | (1<<BIT1);
	
if((PORTF_GPIODR_R -> DATA & 0x01) == 0x01)
{
// This indicates that Hall Sensors 1 - 8 has been triggered	
// And that Atmel board #1 has signalled the Cortex via Pin 0 on PORTF
// The application can now process the interrupt (e.g. fetch the HS# via SPI)
	hall_sensor_input(atmel1);
}
	
if((PORTF_GPIODR_R -> DATA & 0x02) == 0x02)
{
// This indicates that Hall Sensors 9 - 32 has been triggered	
// And that Atmel board #3 has signalled the Cortex via Pin 1 on PORTF
// The application can now process the interrupt (e.g. fetch the HS# via SPI)
	hall_sensor_input(atmel3);
}
	NVIC_INT_CTRL_R |= TRIGGER_PENDSV;
}
