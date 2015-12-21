#include "sysctl.h"


/* 
 * file: "sysctl.c"
 *
 * ECED 4402
 * Author: Emad Khan 
 * 
 * This file contains the following:
 *
 * Declared structures for System Control of Run-Mode
 * Clock Configuration 1 and 2 registers (SYSCTL_RCGC1, SYSCTL_RCGC2)
 * with its corresponding address.
 *
 * Copy this file into your CCS project for ease in porting the rest of the SPI code
 *
 * Date of creation: April 2nd, 2012
 * Date last edited: March 14th, 2013
 * Version: 2
 *
 */

// Structures associated with sysctl.h

struct SYSCTL_RCGC1 * const SYSCTL_RCGC1_R = 
	(struct SYSCTL_RCGC1 *) 0x400FE104;	// Run Mode Clock Gating Control Register 1

struct SYSCTL_RCGC2 * const SYSCTL_RCGC2_R  = 
	(struct SYSCTL_RCGC2 *) 0x400FE108;	// Run Mode Clock Gating Control Register 2

