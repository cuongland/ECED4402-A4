/*
 * railsystem.h
 *
 *  Created on: Dec 2, 2015
 *      Author: Duc Cuong Dinh
 */

#ifndef RAILSYSTEM_H_
#define RAILSYSTEM_H_

#include "process.h"

#define MAX_SECTION	17

enum SECTION_STATE { AVAILABLE, USED, STARTING, ENDING,MASKED};
enum GATE	{NONE, DIRECT};
struct SectionElement
{
	volatile enum SECTION_STATE status;
	volatile unsigned int direction;
	volatile unsigned int speed;
	unsigned int next_segment;
	unsigned int piD;
};

void printfting(void);
void printsectionavailable(void);
#endif /* RAILSYSTEM_H_ */
