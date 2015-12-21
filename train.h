/*
 * train.h
 *
 *  Created on: Dec 8, 2015
 *      Author: Duc Cuong Dinh
 */

#ifndef TRAIN_H_
#define TRAIN_H_

#include "railsystem.h"

enum DIRECTION_OPTION	{CW, CCW, UNUSE};

struct NodeElement
{
	struct NodeElement *prev;
	unsigned int starting;
	unsigned int end;
	unsigned char section[MAX_SECTION];
};

struct TrainElement
{
	unsigned int starting;
	unsigned int ending;
	unsigned int speed;
};

unsigned int IldeQrocess(unsigned int data);
unsigned int making_decision(unsigned int start, unsigned int end, unsigned int old_section);
struct NodeElement * recursion(struct NodeElement *main_head, unsigned int start, struct NodeElement *next_node);
unsigned int Process(unsigned int data);

#endif /* TRAIN_H_ */
