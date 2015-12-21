/*
 * HallSensor.h
 *
 *  Created on: Dec 5, 2015
 *      Author: Duc Cuong Dinh
 */

#ifndef HALLSENSOR_H_
#define HALLSENSOR_H_

#define CONVERT_TO_SECTION(x)	((( (x) - 1 ) >> 1 ) + 1)
#define DIR_SPEED(x, y)			(((x)<<7)|(y))

struct CarryData
{
	unsigned int hsnumber;
	unsigned int next;
};

unsigned int print_hallsensor(unsigned int hsnumber);
unsigned int obtain_decision(unsigned int hsnumber);

unsigned int update_the_next_value(unsigned int data1, unsigned int data2);
unsigned int copy_the_next_value(unsigned int data1, unsigned int data2);
unsigned int delete_the_next_value(unsigned int data1);
void prinths_next(unsigned int h, unsigned int n);
#endif /* HALLSENSOR_H_ */
