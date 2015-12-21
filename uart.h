/*
 * uart.h
 *
 *  Created on: Oct 17, 2015
 *      Author: Duc Cuong Dinh
 */

#ifndef UART_H_
#define UART_H_

/*
 Stellaris UART0 registers
*/

// UART0 & PORTA Registers
#define GPIO_PORTA_AFSEL_R	(*((volatile unsigned long *)0x40004420))	// GPIOA Alternate Function Select Register
#define GPIO_PORTA_DEN_R	(*((volatile unsigned long *)0x4000451C))	// GPIOA Digital Enable Register
#define UART0_DR_R      	(*((volatile unsigned long *)0x4000C000))	// UART0 Data Register
#define UART0_FR_R      	(*((volatile unsigned long *)0x4000C018))	// UART0 Flag Register
#define UART0_IBRD_R   		(*((volatile unsigned long *)0x4000C024))	// UART0 Integer Baud-Rate Divisor Register
#define UART0_FBRD_R     	(*((volatile unsigned long *)0x4000C028))	// UART0 Fractional Baud-Rate Divisor Register
#define UART0_LCRH_R    	(*((volatile unsigned long *)0x4000C02C))	// UART0 Line Control Register
#define UART0_CTL_R    		(*((volatile unsigned long *)0x4000C030))	// UART0 Control Register
#define UART0_IFLS_R    	(*((volatile unsigned long *)0x4000C034))	// UART0 Interrupt FIFO Level Select Register
#define UART0_IM_R      	(*((volatile unsigned long *)0x4000C038))	// UART0 Interrupt Mask Register
#define UART0_MIS_R      	(*((volatile unsigned long *)0x4000C040))	// UART0 Masked Interrupt Status Register
#define UART0_ICR_R      	(*((volatile unsigned long *)0x4000C044))	// UART0 Interrupt Clear Register

#define INT_UART0           	5           // UART0 Rx and Tx interrupt index (decimal)
#define UART_FR_TXFF        	0x00000020  // UART Transmit FIFO Full
#define UART_FR_RXFE        	0x00000010  // UART Receive FIFO Empty
#define UART_FR_BUSY			0x00000008	// UART Transmit busy
#define UART_RX_FIFO_ONE_EIGHT	0x00000000  // UART Receive FIFO Interrupt Level at >= 1/8
#define UART_TX_FIFO_SVN_EIGHT	0x00000000  // UART Transmit FIFO Interrupt Level at <= 7/8
#define UART_LCRH_WLEN_8    	0x00000060  // 8 bit word length
#define UART_LCRH_FEN       	0x00000010  // UART Enable FIFOs
#define UART_CTL_UARTEN     	0x00000301  // UART RX/TX Enable
#define UART_INT_TX         	0x020       // Transmit Interrupt Mask
#define UART_INT_RX         	0x010       // Receive Interrupt Mask
#define UART_INT_RT         	0x040       // Receive Timeout Interrupt Mask
#define UART_CTL_EOT        	0x00000010  // UART End of Transmission Enable
#define EN_RX_PA0           	0x00000001  // Enable Receive Function on PA0
#define EN_TX_PA1           	0x00000002  // Enable Transmit Function on PA1
#define EN_DIG_PA0          	0x00000001  // Enable Digital I/O on PA0
#define EN_DIG_PA1          	0x00000002  // Enable Digital I/O on PA1

// Clock Gating Registers
#define SYSCTL_RCGC1_R		(*((volatile unsigned long *)0x400FE104))
#define SYSCTL_RCGC2_R		(*((volatile unsigned long *)0x400FE108))

#define SYSCTL_RCGC1_UART0  	0x00000001  // UART0 Clock Gating Control
#define SYSCTL_RCGC2_GPIOA  	0x00000001  // Port A Clock Gating Control

// Clock Configuration Register
#define SYSCTRL_RCC_R       	(*((volatile unsigned long *)0x400FE060))

#define CLEAR_USRSYSDIV		0xF83FFFFF	// Clear USRSYSDIV Bits
#define SET_BYPASS		0x00000800	// Set BYPASS Bit

#define NVIC_EN0_R		(*((volatile unsigned long *)0xE000E100))	// Interrupt 0-31 Set Enable Register
#define NVIC_EN1_R		(*((volatile unsigned long *)0xE000E104))	// Interrupt 32-54 Set Enable Register

void UART0_Init(void);
void IntEnable(unsigned long InterruptIndex);
void UART0_IntEnable(unsigned long flags);
void initialization_uart(void);

unsigned int transmitting(void);

unsigned int transmitting_data(unsigned int data);

unsigned int send_to_UART(volatile char *data);

#endif /* UART_H_ */
