#include "gpioe.h"

/* 
 * file: "gpioe.c"
 *
 * ECED 4402
 * Author: Emad Khan 
 * 
 * This file contains the following:
 *
 * - Structure addresses for the various PortE registers.
 *
 * Copy this file into your CCS project for ease in porting the rest of the PortE code
 * 
 * Date of creation: April 2nd, 2012
 * Date last edited: March 14th, 2013
 * Version: 2
 *
 */
 
// Structures associated with General Purpose Input/Output PORT E

struct PORTE_GPIODR * const PORTE_GPIODR_R = 
	(struct PORTE_GPIODR *) 0x400243FC;	// GPIO Data 

struct PORTE_GPIODIR * const PORTE_GPIODIR_R = 
	(struct PORTE_GPIODIR *) 0x40024400;	// GPIO Direction 

struct PORTE_GPIOAFSEL * const PORTE_GPIOAFSEL_R = 
	(struct PORTE_GPIOAFSEL *) 0x40024420; // GPIO Alternate Function Select 

struct PORTE_GPIODEN * const PORTE_GPIODEN_R = 
	(struct PORTE_GPIODEN *) 0x4002451C;	// GPIO Digital Enable

struct PORTE_GPIOPCTL * const PORTE_GPIOPCTL_R = 
	(struct PORTE_GPIOPCTL *) 0x4002452C;	// GPIO Port Control 
