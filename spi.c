#include "gpioe.h"
#include "sysctl.h"
#include "spi.h"

/* 
 * file: "spi.c"
 *
 * ECED 4402
 * Author: Emad Khan 
 * 
 * This file contains the following:
 *
 * - Structure addresses for the various SPI/SSI registers.
 *
 * - SPI_Init(void) - a function called to initalize and Enable the SPI
 * and its significant bits, including Pin Multiplexing (Bits 0 through 3 on PortE) and
 * Slave Select lines (bits 4 through 6 on PortE).
 *
 * - SPIDataTransmit(unsigned char Data, unsigned Atmel_Number) - transmits a byte "Data"
 * to the desired Atmel (1, 2 or 3). Calls WaitSPIBusy() for each transmission.
 *
 * - WaitSPIBusy() - a function that waits until the SPI is free. Called everytime
 * SPIDataTransmit is called.
 *
 * - SPIDataReceive(void) - a function that reads the value that was last shifted into
 * the SPI Data Register. Should be called only after something has been transmitted.
 *
 * Date of creation: April 2nd, 2012
 * Date last edited: March 14th, 2013
 * Version: 2
 *
 */

// Structures associated with spi.h

struct SSI1_CR0 * const SSI1_CR0_R = (struct SSI1_CR0 *) 0x40009000;	// SSI1 Control 0 Register
struct SSI1_CR1 * const SSI1_CR1_R = (struct SSI1_CR1 *) 0x40009004;	// SSI1 Control 1 Register
struct SSI1_DR  * const  SSI1_DR_R = (struct SSI1_DR  *) 0x40009008;	// SSI1 Data Register
struct SSI1_SR  * const  SSI1_SR_R = (struct SSI1_SR  *) 0x4000900C;	// SSI1 Status Register
struct SSI1_CPSR * const SSI1_CPSR_R = (struct SSI1_CPSR *) 0x40009010; // SSI1 Clock Prescale Register
struct SSI1_IM  * const SSI1_IM_R  = (struct SSI1_IM  *) 0x40009014;	// SSI1 Interrupt Mask Register
struct SSI1_MIS * const SSI1_MIS_R = (struct SSI1_MIS *) 0x4000901C;	// SSI1 Masked Interrupt Status Register
struct SSI1_ICR * const SSI1_ICR_R = (struct SSI1_ICR *) 0x40009020;	// SSI1 Interrupt Clear Register

void SPI_Init(void)	// initializes SPI
{
	// Activate SSI1
	SYSCTL_RCGC1_R -> SSI1 = ACTIVATE; 		
	
	// Activate Port E
	SYSCTL_RCGC2_R -> GPIOE = ACTIVATE;		
	
	// Activate alternate function select for Pins E0, E2, E3 
	PORTE_GPIOAFSEL_R -> AFSEL |= (PIN0 | PIN2 | PIN3);	
	PORTE_GPIOAFSEL_R -> AFSEL &= ~( (1<<BIT4) | (1<<BIT5) | (1<<BIT6) );
	
	// Set Pin Multiplexing (PMCx Bit Field = 2) for Pins E0, E2, E3
	PORTE_GPIOPCTL_R -> PMC0 = 0x02;
	PORTE_GPIOPCTL_R -> PMC2 = 0x02;
	PORTE_GPIOPCTL_R -> PMC3 = 0x02;
	
	// Set Pins E4, E5, E6 as output for Slave Selects 1, 2, 3
	PORTE_GPIODIR_R -> DIR |= ( (1<<BIT4) | (1<<BIT5) | (1<<BIT6) );
	
	// Set all pins on Port E to Digital I/O
	PORTE_GPIODEN_R -> DEN = ALL_PINS; 
	
	// Set all Slave Selects High
	PORTE_GPIODR_R -> DATA |= ( (1<<BIT4) | (1<<BIT5) | (1<<BIT6) );
	
	// Disable SSI
	SSI1_CR1_R -> SSE = DISABLE; 
  
  	// Set Master Mode
  	SSI1_CR1_R -> MS = CLEAR;
  	  
  	// Set CPSDVSR = 160 for Master Clock of 100KHz
  	SSI1_CPSR_R -> CPSDVSR = 160;
  	
  	// Set SCR = 0, SPH = 0, SPO = 0
  	SSI1_CR0_R -> SCR = 0;
  	SSI1_CR0_R -> SPH = 0;
  	SSI1_CR0_R -> SPO = 0;
  
  	// Set Data Size to 8 bits
  	SSI1_CR0_R -> DSS = 0x7;
  	
  	// Set Frame Format to SPI
  	SSI1_CR0_R -> FRF = 0;
  	
  	// Enable SSI
  	SSI1_CR1_R -> SSE = ENABLE;
} 

void SPIDataTransmit(unsigned char Data, unsigned Atmel_Number)
{
unsigned volatile Atmel = Atmel_Number;
	
if(Atmel <=3 && Atmel >= 1)
{
	// Bring SS Low for the Atmel
	PORTE_GPIODR_R -> DATA &= ~(1 << (Atmel+3));
	
	while(!(SSI1_SR_R -> TNF))
	{
    		// Wait until there is space in Transmit FIFO
	}
	
	// Transmit the Character via the Data Register
	SSI1_DR_R -> DATA = Data;
    
	WaitSPIBusy();
    
	// Bring SS High for the Atmel
	PORTE_GPIODR_R -> DATA |= (1 << (Atmel+3));
	
}
}

unsigned char SPIDataReceive(void)
{
// Wait until character is received in Receive FIFO
while(!(SSI1_SR_R -> RNE)) ;

// Return the 8 bit character from the 16 bit register
return (SSI1_DR_R -> DATA & 0xFF);
}

void WaitSPIBusy(void)
{ 
// Wait until SSI is free
while(SSI1_SR_R -> BSY) ;

}
