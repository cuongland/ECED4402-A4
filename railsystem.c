/*
 * railsystem.c
 *
 *  Created on: Dec 2, 2015
 *      Author: Duc Cuong Dinh
 */
#include <stdio.h>
#include "railsystem.h"
#include "train.h"
#include "uart.h"
#include "memory.h"
#include "cursor.h"
extern enum DIRECTION_OPTION select_direction[MAX_SECTION][MAX_SECTION];

volatile struct SectionElement railsystem[MAX_SECTION] = {{USED,0,0,0,0},
												{AVAILABLE,0,0,0,0},{AVAILABLE,0,0,0,0},{AVAILABLE,0,0,0,0},{AVAILABLE,0,0,0,0},
												{AVAILABLE,0,0,0,0},{AVAILABLE,0,0,0,0},{AVAILABLE,0,0,0,0},{AVAILABLE,0,0,0,0},
												{AVAILABLE,0,0,0,0},{AVAILABLE,0,0,0,0},{AVAILABLE,0,0,0,0},{AVAILABLE,0,0,0,0},
												{AVAILABLE,0,0,0,0},{AVAILABLE,0,0,0,0},{AVAILABLE,0,0,0,0},{AVAILABLE,0,0,0,0}};
enum GATE map_gate[MAX_SECTION][MAX_SECTION] = {
	//
		{NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE}, 	//0
	//         		  2                         6											13
		{NONE, NONE, DIRECT, NONE, NONE, NONE, DIRECT, NONE, NONE, NONE, NONE, NONE, NONE, DIRECT, NONE, NONE, NONE},	//1
	//      	1             3                                                                   14
		{NONE, DIRECT, NONE, DIRECT, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, DIRECT, NONE, NONE}, //2
	//					 2				4						8
		{NONE, NONE, DIRECT, NONE, DIRECT, NONE, NONE, NONE, DIRECT, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE}, //3
	//						3			5													13					16
		{NONE, NONE, NONE, DIRECT, NONE, DIRECT, NONE, NONE, NONE, NONE, NONE, NONE, NONE, DIRECT, NONE, NONE, DIRECT}, //4
	//								 4				6									12
		{NONE, NONE, NONE, NONE, DIRECT, NONE, DIRECT, NONE, NONE, NONE, NONE, NONE, DIRECT, NONE, NONE, NONE, NONE}, //5
	//			1						 5
		{NONE, DIRECT, NONE, NONE, NONE, DIRECT, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE}, //6
	//														 8						9					15
		{NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, DIRECT, NONE, NONE, NONE, DIRECT, NONE, NONE, DIRECT, NONE}, //7
	//						3						7				9
		{NONE, NONE, NONE, DIRECT, NONE, NONE, NONE, DIRECT, NONE, DIRECT, NONE, NONE, NONE, NONE, NONE, NONE, NONE}, //8
	//												  		8				10
		{NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, DIRECT, NONE, DIRECT, NONE, NONE, NONE, NONE, NONE, NONE}, //9
	//															9			  11
		{NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, DIRECT, NONE, DIRECT, NONE, NONE, NONE, NONE, NONE}, //10
	//															  		10			12
		{NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, DIRECT, NONE, DIRECT, NONE, NONE, NONE, NONE}, //11
	//									5			 	7						11
		{NONE, NONE, NONE, NONE, NONE, DIRECT, NONE, DIRECT, NONE, NONE, NONE, DIRECT, NONE, NONE, NONE, NONE, NONE}, //12
	//			1					4
		{NONE, DIRECT, NONE, NONE, DIRECT, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE}, //13
	//					2
		{NONE, NONE, DIRECT, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE}, //14
	//												 7
		{NONE, NONE, NONE, NONE, NONE, NONE, NONE, DIRECT, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE}, //15
	//						 		4
		{NONE, NONE, NONE, NONE, DIRECT, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE} //16
};

void printfting(void)
{
	unsigned int i,j;
	for(i=0;i<MAX_SECTION;i++)
	{
		printf("%d: ",i);
		for(j=0;j<MAX_SECTION;j++)
			if(map_gate[i][j] != NONE)
				printf("(%d-%d ) ",j,select_direction[i][j]);
		printf("\n");
	}
}

void printsectionavailable(void)
{
	char *temp;
	unsigned int location;
	location = 0;
	temp = (char *)allocate(256);
	cursor_position(temp, &location, 5,1);

	unsigned int i;
	for(i=0;i<MAX_SECTION;i++)
	{
		temp[location++] = '<';
		temp[location++] = i+'0';
		temp[location++] = '-';
		temp[location++] = railsystem[i].status+'0';
		temp[location++] = '-';
		temp[location++] = '0'+ railsystem[i].piD;
		temp[location++] = '-';
		temp[location++] = '0'+ railsystem[i].direction;
		temp[location++] = '-';
		temp[location++] = '0'+ railsystem[i].speed;
		temp[location++] = '-';
		temp[location++] = '0'+ railsystem[i].next_segment/10;
		temp[location++] = '0'+ railsystem[i].next_segment%10;
		temp[location++] = '>';
		temp[location++] = ' ';
	}
	temp[location++] = 0xFF;
	send_to_UART(temp);
}
