/*
 * uart.c
 *
 *  Created on: Oct 17, 2015
 *      Author: Duc Cuong Dinh
 */

/*
 * UART test code:
 * Should echo input from the keyboard on the UART console
 * Caveat emptor!
 * 30 Jan 2013
 *
 * This code works and echoes characters.  Neither the FIFOs nor the Receive-Timer interrupt
 * are used.  This makes the UART function as a run-of-the-mill one-byte-one-interrupt UART.
 * 4 Feb 2013
 */
#include <stdio.h>
#include <stdlib.h>

#include "uart.h"
#include "Queue.h"
#include "PendSVHandler.h"
#include "ControlSoftware.h"
#include "memory.h"

/* Globals */
volatile char Data_RX;  	/* Output data from UART receive */

volatile char *buffer_TX;

void SetupPIOSC(void)
{
/* Set BYPASS, clear USRSYSDIV and SYSDIV */
SYSCTRL_RCC_R = (SYSCTRL_RCC_R & CLEAR_USRSYSDIV) | SET_BYPASS ;	// Sets clock to PIOSC (= 16 MHz)
}

void initialization_uart(void)
{
/* Initialize UART */
SetupPIOSC();			// Set Clock Frequency to 16MHz (PIOSC)
UART0_Init();			// Initialize UART0
IntEnable(INT_UART0);		// Enable UART0 interrupts
UART0_IntEnable(UART_INT_RX | UART_INT_TX);	// Enable Receive and Transmit interrupts

buffer_TX = NULL;
}

void UART0_Init(void)
{
/* Initialize UART0 */
SYSCTL_RCGC1_R |= SYSCTL_RCGC1_UART0; 	// Enable Clock Gating for UART0
SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA; 	// Enable Clock Gating for PORTA

UART0_CTL_R &= ~UART_CTL_UARTEN;      	// Disable the UART

// Setup the BAUD rate
UART0_IBRD_R = 8;	// IBRD = int(16,000,000 / (16 * 115,200)) = int(8.68055)
UART0_FBRD_R = 44;	// FBRD = int(0.68055 * 64 + 0.5) = 44.0552
UART0_LCRH_R = (UART_LCRH_WLEN_8);	// WLEN: 8, no parity, one stop bit, without FIFOs)

UART0_CTL_R |= UART_CTL_UARTEN;        // Enable the UART and End of Transmission Interrupts

GPIO_PORTA_AFSEL_R |= EN_RX_PA0 | EN_TX_PA1;    	// Enable Receive and Transmit on PA1-0
GPIO_PORTA_DEN_R |= EN_DIG_PA0 | EN_DIG_PA1;   		// Enable Digital I/O on PA1-0
}

void IntEnable(unsigned long InterruptIndex)
{
/* Indicate to CPU which device is to interrupt */
if(InterruptIndex < 32)
	NVIC_EN0_R = 1 << InterruptIndex;		// Enable the interrupt in the EN0 Register
else
	NVIC_EN1_R = 1 << (InterruptIndex - 32);	// Enable the interrupt in the EN1 Register
}

void UART0_IntEnable(unsigned long flags)
{
/* Set specified bits for interrupt */
UART0_IM_R |= flags;
}

//function name: send_a_char
//the function will take a character from buffer_TX and send to UART0_DR_R
//if buffer_TX[0] is NULL_INDEX (0xFF), then UART0_DR_R will be filled 0x0 (NUL)
//input: no parameter input, it takes BUffer_TX
//output: if buffer_TX is NULL, output is FALSE
//		  if buffer_TX is not NULL, output is TRUE
unsigned int send_a_char(void)
{
	//check buffer_TX, if it is NULL, then return FALSE right aways
	if(buffer_TX==NULL)
		return FALSE;

	//when buffer_TX is not NULL, there are still some characters left to send
	//checking transmition is busy or not
	if((UART0_FR_R & UART_FR_BUSY)==0)
	{
		//check buffer_TX[0] is NULL_INDEX (0xFF) or not
		if(buffer_TX[0] != NULL_INDEX)
		{
			// if not, send the character to UART0_DR_R
			UART0_DR_R = buffer_TX[0];
			//move to the next character
			buffer_TX++;
		}
		else
		{
			//if yes, UART0_DR_R will got NULL (0x0)
			UART0_DR_R = NUL;
			//deallocate buffer_TX for memory
			deallocate((unsigned int *)buffer_TX);
			//set the buffer_TX to be back to NULL
			buffer_TX = NULL;
		}
	}
	//return TRUE for Queue to wait until transmition is completed
	return TRUE;
}

//the task transmitting_data is used to send characters to UART0_DR_R
//it will be held until the Buffer_TX is set NULL, the string is sent
//if buffer_TX is not NULL, send_a_char() is still reuturned TRUE
//if buffer_TX is NULL, send_a_char will return FALSE so that QUeue can process
//the next task
unsigned int transmitting_data(unsigned int data)
{
	while(send_a_char())
		return TRUE;

	return FALSE;
}

//get a string from the process or from any other sources
//input: a string, the address of the string in memory
//output: FALSE: if the queue is FULL,
//		  TRUE: if QUEUE is not FULL
unsigned int send_to_UART(volatile char *data)
{
	//getting the string
	//in the simulation, where uart does not work, this code will handle
	// memory will not be overload
	if(buffer_TX!=NULL)
		if(deallocate((unsigned int *)buffer_TX))
			return FALSE;
	buffer_TX = data;
//	buffer_TX[127] = 0xFF;
	//add the task to queue

	//add task to queue
	if(enqueue(0,transmitting_data) == FALSE)
		return FALSE;

	//call PendSV handler to process the transmit
	NVIC_INT_CTRL_R |= TRIGGER_PENDSV;
	return TRUE;
}


void UART0_IntHandler(void)
{
/*
 * Simplified UART ISR - handles receive and xmit interrupts
 * Application signalled when data received
 */

	if (UART0_MIS_R & UART_INT_RX)
	{
		/* RECV done - clear interrupt and make char available to application */
		UART0_ICR_R |= UART_INT_RX;
		Data_RX = (char)UART0_DR_R;

		//the task to queue
		enqueue(Data_RX,get_received_char);

		//call PendSV handler to process the transmit
		NVIC_INT_CTRL_R |= TRIGGER_PENDSV;
	}

	if (UART0_MIS_R & UART_INT_TX)
	{
		/* XMIT done - clear interrupt */
		UART0_ICR_R |= UART_INT_TX;
	}

}
