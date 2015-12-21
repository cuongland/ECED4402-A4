/*
 * kernal.h
 *
 *  Created on: Nov 15, 2015
 *      Author: Duc Cuong Dinh
 */

#ifndef KERNAL_H_
#define KERNAL_H_

enum kernelcallcodes {GETID, NICE, TERMINATE, PROMOTEBY, DEMOTEBY};

struct kcallargs
{
	unsigned int code;
	unsigned int rtnvalue;
	unsigned int arg1;
	unsigned int arg2;
};

unsigned int getId(void);
unsigned int terminate(void);
unsigned int promoteby(unsigned int increment);
unsigned int demoteby(unsigned int decrement);

#endif /* KERNAL_H_ */
