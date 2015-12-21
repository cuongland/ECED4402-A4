/*
 * HallSensor.c
 *
 *  Created on: Dec 5, 2015
 *      Author: Duc Cuong Dinh
 */
#include <stdio.h>
#include "generalinfo.h"
#include "memory.h"
#include "cursor.h"
#include "uart.h"
#include "HallSensor.h"
#include "kernel.h"
#include "messageatmel.h"
#include "switch.h"
#include "semaphore.h"
#include "railsystem.h"
#include "PendSVHandler.h"
#include "Queue.h"

extern volatile struct SectionElement railsystem[MAX_SECTION];
extern volatile enum SWITCH_STATE gate[MAX_SWITCH];

//printing the hallsensor to uart
unsigned int print_hallsensor(unsigned int hsnumber)
{
	volatile char *hs_result;
	volatile unsigned int location;
	hs_result = (char *)allocate(STRING_SIZE);

	if(hs_result == NULL)
		return FALSE;

	location = 0;
	cursor_position(hs_result,&location,hsnumber*6,2);
//	hs_result[location++] = getId() + '0';

	hs_result[location++] = hsnumber /10 + '0';
	hs_result[location++] = hsnumber %10 + '0';

	hs_result[location++] = (char)NULL_INDEX;//the end of string
	send_to_UART(hs_result);
	hs_result = NULL;
	return TRUE;
}

//obtain_decision:
//input: hallsensor
//noput: NO
//purpose: the function will do certain jobs bases on the hallsensor and direction
//if the segment is over, then turn it off , and if the segment with the switch
//		then turn off the switch as well.
//if the segment is start to enter, copy the information to next segment
//the variable NEXT: is used to determent which hallsensor is next.
//these information is based on the direction for each hallsensor.
unsigned int obtain_decision(unsigned int hsnumber)
{
	unsigned int direction = railsystem[CONVERT_TO_SECTION(hsnumber)].direction;
	unsigned int next;

	switch(hsnumber)
	{
		case 1: 
			if(direction)
			{
				next = 2;
				hall_sensor_dir_mag(6,0);
			}
			else
			{
				next = 12;
				update_the_next_value(hsnumber,next);
			}
			break;
		case 2:
			if(direction)
			{
				if(gate[1]==ON)
					next=25;
				else
					next=3;
				update_the_next_value(hsnumber,next);
			}
			else
			{
				hall_sensor_dir_mag(2,0);
				if(gate[1]==ON)
				{
					hall_sensor_switch(1,1);
					gate[1]==OFF;
					hall_sensor_dir_mag(13,0);
					hall_sensor_dir_mag(2,0);
				}
				next = 1;
			}
			break;
		case 3:
			if(direction)
			{
				hall_sensor_dir_mag(1,0);
				next = 4;
			}
			else
			{
				next = 2;
				if(gate[2]==ON)
				{
					gate[2]=OFF;
					hall_sensor_switch(1,1);
					hall_sensor_dir_mag(14,0);
					hall_sensor_dir_mag(8,0);
				}
				update_the_next_value(hsnumber,next);
			}
			break;
		case 4:
			if(direction)
			{
				next = 5;
				update_the_next_value(hsnumber,next);
			}
			else
			{
				next = 3;
				hall_sensor_dir_mag(3,0);
			}
			break;

		case 5:
			if(direction)
			{
				hall_sensor_dir_mag(2,0);
				next = 6;
			}
			else
			{
				next = 4;
				update_the_next_value(hsnumber,next);
			}
			break;

		case 6:
			if(direction)
			{
				next = 7;
				update_the_next_value(hsnumber,next);
				if(gate[3]==ON)
				{
					hall_sensor_dir_mag(9,0);
					hall_sensor_dir_mag(8,0);
					gate[3]=OFF;
					gate[7]=OFF;
					hall_sensor_switch(3,1);
					hall_sensor_switch(7,1);
				}
			}
			else
			{
				hall_sensor_dir_mag(4,0);
				if(gate[3]==ON)
				{
					next = 16;
					update_the_next_value(hsnumber,next);
				}
				else
					next = 5;
			}
			break;

		case 7:
			if(direction)
			{
				next=8;
				hall_sensor_dir_mag(3,0);
			}
			else
			{
				if(gate[4]== ON)	//s4
				{
					next = 26;
					update_the_next_value(hsnumber,next);
				}
				else
				{
					next = 6;
					update_the_next_value(hsnumber,next);
				}
			}
			break;
		case 8:
			if(direction)
			{
			  if(gate[5]== ON)
				{
				  	next = 31;
				  	update_the_next_value(hsnumber,next);
				}
				else
				{
					next = 9;
					update_the_next_value(hsnumber,next);
				}
			}
			else
			{
				next=7;
				if(gate[5]==ON)
				{
					hall_sensor_dir_mag(16,0);
				}
				else
				{
					hall_sensor_dir_mag(5,0);
				}
			}
			break;
		case 9:
			if(direction)
			{
				hall_sensor_dir_mag(4,0);
				next=10;
			}
			else
			{
				next=8;
				update_the_next_value(hsnumber,next);
				if(gate[6]==ON)
				{
					gate[6] = OFF;
					gate[8] = OFF;
					hall_sensor_switch(6,1);
					hall_sensor_switch(8,1);
					hall_sensor_dir_mag(12,0);
					delete_the_next_value(24);
					hall_sensor_dir_mag(12,0);
				}
			}
			break;

		case 10:
			if(direction)
			{
				next=11;
				update_the_next_value(hsnumber,next);
			}
			else
			{
				hall_sensor_dir_mag(6,0);
				next=9;
			}

			break;

		case 11:
			if(direction)
			{
				hall_sensor_dir_mag(5,0);
				next=12;
			}
			else
			{
				next=10;
				update_the_next_value(hsnumber,next);
			}
			break;

		case 12:
			if(direction)
			{
				next=1;
				update_the_next_value(hsnumber,next);
			}
			else
			{
				hall_sensor_dir_mag(1,0);
				next=11;
			}
			break;

		case 13:
			if(direction)
			{
				next=14;
				hall_sensor_dir_mag(12,0);
				if(gate[9]==ON)
				{
					hall_sensor_dir_mag(15,0);
					gate[9] = OFF;
					hall_sensor_switch(9,1);
				}
			}
			else
			{
				if(gate[9]==ON)
					next = 29;
				else
					next = 24;
				update_the_next_value(hsnumber,next);
			}
			break;
		case 14:
			if(direction)
			{
				next=15;
				update_the_next_value(hsnumber,next);
			}
			else
			{
				hall_sensor_dir_mag(8,0);
				next=13;
			}
			break;
		case 15:
			if(direction)
			{
				next=16;
				hall_sensor_dir_mag(7,0);
			}
			else
			{
				next=14;
				update_the_next_value(hsnumber,next);
			}
			break;
		case 16:
			if(direction)
			{
				if(gate[7]==ON)
				{
					next = 6;
					copy_the_next_value(hsnumber,next);
				}
				else
				{
					next = 17;
					update_the_next_value(hsnumber,next);
				}
			}
			else
			{
				hall_sensor_dir_mag(9,0);
				next=15;
				if(gate[7]==ON)
				{
					hall_sensor_dir_mag(3,0);
					gate[7]=OFF;
					gate[3]=OFF;
					hall_sensor_switch(7,1);
					hall_sensor_switch(3,1);
				}
			}
			printf("<<%d>>",next);
			break;
		case 17:
			if(direction)
			{
				hall_sensor_dir_mag(8,0);
				next=18;
			}
			else
			{
				next=16;
				update_the_next_value(hsnumber,next);
			}
			break;
		case 18:
			if(direction)
			{
				next=19;
				update_the_next_value(hsnumber,next);
				semaPost((unsigned int*)&(railsystem[3].status));
			}
			else
			{
				hall_sensor_dir_mag(10,0);
				next=17;
			}
			break;
		case 19:
			if(direction)
			{
				hall_sensor_dir_mag(9,0);
				next=20;
			}
			else
			{
				next=18;
				update_the_next_value(hsnumber,next);
			}
			break;

		case 20:
			if(direction)
			{
				next=21;
				update_the_next_value(hsnumber,next);
			}
			else
			{
				hall_sensor_dir_mag(11,0);
				next=19;
			}
			break;
		case 21:
			if(direction)
			{
				hall_sensor_dir_mag(10,0);
				next=22;
			}
			else
			{
				next=20;
				update_the_next_value(hsnumber,next);
			}
			break;
		case 22:
			if(direction)
			{
				next=23;
				update_the_next_value(hsnumber,next);
			}
			else
			{
				hall_sensor_dir_mag(12,0);
				next=21;
			}
			break;
		case 23:
			if(direction)
			{
				hall_sensor_dir_mag(11,0);
				next=24;
			}
			else
			{
				next=22;
				update_the_next_value(hsnumber,next);
			}
			break;
		case 24:
			if(direction)
			{
				next=13;
				update_the_next_value(hsnumber,next);
				if(gate[8]==ON)
				{
					gate[8]=OFF;
					gate[6]=OFF;
					hall_sensor_switch(8,1);
					hall_sensor_switch(6,1);
					delete_the_next_value(9);
					hall_sensor_dir_mag(5,0);
				}
			}
			else
			{
				hall_sensor_dir_mag(7,0);
				next=23;
			}
			break;

		case 25:
			if(direction)
			{
				next=26;
				hall_sensor_dir_mag(2,0);
				hall_sensor_dir_mag(1,0);
			}
			else
			{
				next=2;
			}
			break;

		case 26:
			if(direction)
			{
				next=7;
			}
			else
			{
				next=25;
				hall_sensor_dir_mag(3,0);
				hall_sensor_dir_mag(4,0);
			}
			break;

		case 27:
			if(direction)
			{
				next=28;
				hall_sensor_dir_mag(2,0);
				semaPost((unsigned int *)&(railsystem[8].status));
				update_the_next_value(3,hsnumber);
			}
			else
			{
				next=3;
				update_the_next_value(hsnumber,next);
			}
			break;
			
		case 28:
			hall_sensor_dir_mag(14,0);
			break;

		case 29:
			if(direction)
			{
				next=13;
				printf("hehe");
				update_the_next_value(hsnumber,next);
			}
			else
			{
				printf("hehe123");
				hall_sensor_dir_mag(12,0);
				hall_sensor_dir_mag(7,0);
				gate[9]==OFF;
				hall_sensor_switch(9,1);
				next=30;
			}
			break;

		case 30:
			hall_sensor_dir_mag(15,0);
			break;

		case 31:
			if(direction)
			{
				hall_sensor_dir_mag(4,0);
				gate[5]==OFF;
				hall_sensor_switch(5,1);
				next=32;
			}
			else
			{
				next=8;
			}
			break;

		case 32:
			hall_sensor_dir_mag(16,0);
			break;

		default:
			break;
	}
	NVIC_INT_CTRL_R |= TRIGGER_PENDSV;
	return next;
}

//updating the current information to the next segment
//in the end, piD must be unique in the railsystem.
unsigned int update_the_next_value(unsigned int data1, unsigned int data2)
{
	prinths_next(data1,data2);
	railsystem[CONVERT_TO_SECTION(data2)].direction = railsystem[CONVERT_TO_SECTION(data1)].direction;
	railsystem[CONVERT_TO_SECTION(data2)].piD = railsystem[CONVERT_TO_SECTION(data1)].piD;
	railsystem[CONVERT_TO_SECTION(data2)].speed = railsystem[CONVERT_TO_SECTION(data1)].speed;
	railsystem[CONVERT_TO_SECTION(data2)].next_segment = 0;

	railsystem[CONVERT_TO_SECTION(data1)].speed = 0;
	railsystem[CONVERT_TO_SECTION(data1)].piD = 0;
	railsystem[CONVERT_TO_SECTION(data1)].direction = 0;
	printsectionavailable();
	NVIC_INT_CTRL_R |= TRIGGER_PENDSV;
	return FALSE;
}

//copy information is similar to update one
//mainly it is just copy direction and speed
unsigned int copy_the_next_value(unsigned int data1, unsigned int data2)
{
	prinths_next(data1,data2);
	railsystem[CONVERT_TO_SECTION(data2)].direction = railsystem[CONVERT_TO_SECTION(data1)].direction;
	railsystem[CONVERT_TO_SECTION(data2)].speed = railsystem[CONVERT_TO_SECTION(data1)].speed;
	railsystem[CONVERT_TO_SECTION(data2)].next_segment = 0;

	printsectionavailable();
	NVIC_INT_CTRL_R |= TRIGGER_PENDSV;
	return FALSE;
}

//erase the information in particular segment
unsigned int delete_the_next_value(unsigned int data1)
{
	railsystem[CONVERT_TO_SECTION(data1)].direction = 0;
	railsystem[CONVERT_TO_SECTION(data1)].piD = 0;
	railsystem[CONVERT_TO_SECTION(data1)].speed = 0;
	railsystem[CONVERT_TO_SECTION(data1)].next_segment = 0;

	printsectionavailable();
	NVIC_INT_CTRL_R |= TRIGGER_PENDSV;
	return FALSE;
}

//for testing
//it will be sent to uart the current hallsensor and the target one
void prinths_next(unsigned int h, unsigned int n)
{
	char *temp;
	temp = (char *) allocate(128);
	unsigned int location;
	location = 0;
	cursor_position(temp,&location, 20,1);
	temp[location++] = (h/10) + '0';
	temp[location++] = (h%10) + '0';
	temp[location++] = '-';
	temp[location++] = (n/10) + '0';
	temp[location++] = (n%10) + '0';
	temp[location++] = 0xFF;
	send_to_UART(temp);
}
