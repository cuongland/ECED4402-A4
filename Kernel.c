/*
 * kernal.c
 * this is used to enter the SVC
 *  Created on: Nov 15, 2015
 *      Author: Duc Cuong Dinh
 */

#include <stdio.h>
#include "Kernel.h"
#include "generalinfo.h"
#include "process.h"

////////////////////////////////////
//these just used for testing
extern volatile struct PCB *running;
extern volatile struct ManagementOfQueueElement *ManagementOfQueues;
////////////////////////////////////////////////

//create a space and go to svc to get pID
//no input
//output is currently pID
unsigned int getId(void)
{
	/* Volatile to actually reserve space on stack */
	volatile struct kcallargs kargs;
	kargs . code = GETID;

	/* Assign address of getidarg to R7 */
	assignR7((unsigned long) &kargs);

	SVC();

	return kargs . rtnvalue;
}

//create a space and go to svc to delete the PCB
//no input
//output is pID that is terminated
unsigned int terminate(void)
{
	/* Volatile to actually reserve space on stack */
	volatile struct kcallargs kargs;
	kargs.code = TERMINATE;

	/* Assign address of kargs to R7 */
	assignR7((unsigned long) &kargs);

	SVC();

	return kargs . rtnvalue;
}

//create a space and go to svc to put higher priority for the PCB
//no input
//output is pID that is move up
unsigned int promoteby(unsigned int increment)
{
	/* Volatile to actually reserve space on stack */
	volatile struct kcallargs kargs;
	kargs.code = PROMOTEBY;
	kargs.arg1 = increment;
	/* Assign address of kargs to R7 */
	assignR7((unsigned long) &kargs);
	SVC();

	return kargs.rtnvalue;
}

//create a space and go to svc to put lower priority for the PCB
//no input
//output is pID that is move down
unsigned int demoteby(unsigned int decrement)
{
	/* Volatile to actually reserve space on stack */
	volatile struct kcallargs kargs;
	kargs.code = DEMOTEBY;
	kargs.arg1 = decrement;
	/* Assign address of kargs to R7 */
	assignR7((unsigned long) &kargs);
	SVC();

	return kargs.rtnvalue;
}
