/*
 * thread.c
 *
 *  Created on: Nov 17, 2015
 *      Author: Duc Cuong Dinh
 */
#include <stdio.h>

#include "generalinfo.h"
#include "memory.h"
#include "train.h"
#include "railsystem.h"
#include "semaphore.h"
#include "switch.h"
#include "messageatmel.h"
#include "Kernel.h"
#include "HallSensor.h"
#include "PendSVhandler.h"
extern struct SectionElement railsystem[MAX_SECTION];
extern enum GATE map_gate[MAX_SECTION][MAX_SECTION];
extern volatile enum SWITCH_STATE gate[MAX_SWITCH];

enum DIRECTION_OPTION select_direction[MAX_SECTION][MAX_SECTION] = {
		{UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE}, //0
	//					2						  6												13
		{UNUSE, UNUSE, CCW, UNUSE, UNUSE, UNUSE, CW, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, CCW, UNUSE, UNUSE, UNUSE}, //1
	//			1			3																		   14
		{UNUSE, CW, UNUSE, CCW, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, CCW, UNUSE, UNUSE}, //2
	//					2			4						8
		{UNUSE, UNUSE, CW, UNUSE, CCW, UNUSE, UNUSE, UNUSE, CW, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE}, //3
	//							3			5													13					16
		{UNUSE, UNUSE, UNUSE, CW, UNUSE, CCW, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, CW, UNUSE, UNUSE, CCW}, //4
	//								  4			6										12
		{UNUSE, UNUSE, UNUSE, UNUSE, CW, UNUSE, CCW, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, CCW, UNUSE, UNUSE, UNUSE, UNUSE}, //5
	//			1							5
		{UNUSE, CCW, UNUSE, UNUSE, UNUSE, CW, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE}, //6
	//																8						12				  15
		{UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, CCW, UNUSE, UNUSE, UNUSE, CW, UNUSE, UNUSE, CW, UNUSE}, //7
	//							3						7			9
		{UNUSE, UNUSE, UNUSE, CCW, UNUSE, UNUSE, UNUSE, CW, UNUSE, CCW, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE}, //8
	//															  8			10
		{UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, CW, UNUSE, CCW, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE}, //9
	//																	9			11
		{UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, CW, UNUSE, CCW, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE}, //10
	//																			10			12
		{UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, CW, UNUSE, CCW, UNUSE, UNUSE, UNUSE, UNUSE}, //11
	//										5			7						 11
		{UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, CW, UNUSE, CCW, UNUSE, UNUSE, UNUSE, CW, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE}, //12
	//			1					4
		{UNUSE, CCW, UNUSE, UNUSE, CW, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE}, //13
	//					2
		{UNUSE, UNUSE, CW, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE}, //14
	//														7
		{UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, CCW, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE}, //15
	//								 4
		{UNUSE, UNUSE, UNUSE, UNUSE, CW, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE} //16
};

unsigned int IldeQrocess(unsigned int data)
{
	while(1);
	return TRUE;
}

//getting the information from the train(current location, destination, and the last segment)
//it will copy information from the railsystem for which one is available
//based on the its, it will detect how to get to the destination based on the linking between segment(map_gate)
unsigned int making_decision(unsigned int start, unsigned int end, unsigned int old_section)
{
	//init
	unsigned int result;
	struct NodeElement *head;
	struct NodeElement *tail;
	struct NodeElement *temp;
	volatile struct NodeElement *shifting;
	unsigned int count;
	head = (struct NodeElement *)allocate(2024);
	if(head==NULL)
		return FALSE;
	//create the information
	shifting = head;
	shifting++;
	head->prev = NULL;
	head->end = end;
	head->starting = start;
	for(count=0;count<MAX_SECTION;count++)
		head->section[count] = railsystem[count].status;//AVAILABLE;
	head->section[old_section] = MASKED;
	head->section[start] = STARTING;
	head->section[end] = ENDING;

	//scanning
	tail = recursion(head,start,(struct NodeElement *)shifting);
	temp = tail;
	//done
	printf("move: ");
	while(temp)
	{
		printf("%d -> ",temp->starting);
		temp = temp->prev;
	}
	//end
	temp = tail;
	for(temp = tail; temp != NULL && (temp->prev)->starting != start; temp = temp->prev);
	result = temp->starting;

	deallocate((unsigned int *)head);

	return result;
}
//recursion is used to catch_ and try the direction
//we have only 16 segments, so that it is only 16 level deeps
struct NodeElement * recursion(struct NodeElement *main_head, unsigned int start, struct NodeElement *next_node)
{
	struct NodeElement *result;
	unsigned int count;
	unsigned int actualvalue;
	struct NodeElement *head = (struct NodeElement *)next_node;
	head->prev = main_head;
	head->end = main_head->end;
	head->starting = start;

	next_node++;

	if(start == main_head->end)
	{
		return head;
	}

	result = NULL;
	//copying the section
	for(count=0;count<MAX_SECTION;count++)
		head->section[count] = main_head->section[count];
	head->section[start] = MASKED;

	count = 0;
	actualvalue = count;
//condition when the gate enter a segment that has a switch, in certain level,
//the train can not take the switch,
//e.g.: when the train go from segment 3 to segment 2, it can must go to segment 1
	if((head->starting ==2 && main_head->starting == 3))
		actualvalue = 1;

	if((head->starting ==3 && main_head->starting == 2))
		actualvalue = 4;

//	if(head->starting == 9 && main_head->starting == 10)
//		actualvalue = 8;

	if(head->starting == 12 && main_head->starting == 11)
		actualvalue = 7;

	if(head->starting == 5 && main_head->starting == 6)
		actualvalue = 4;

	if((head->starting ==12 && main_head->starting == 5))
		actualvalue = 7;

	if((head->starting == 5 && main_head->starting == 12))
		actualvalue = 4;

	if((head->starting == 3 && main_head->starting == 8))
		actualvalue = 4;

	if((head->starting == 8 && main_head->starting == 3))
		actualvalue = 7;
//if the segment is not zero
	if(actualvalue)
	{
		if(map_gate[start][actualvalue] == DIRECT)
		{
			if(head->section[actualvalue] == ENDING)
			{
				result = recursion(head,actualvalue,next_node);
			}
			else
			if(head->section[actualvalue] == AVAILABLE)
			{
				result = recursion(head,actualvalue,next_node);
				if(result != NULL)
				{
					if( result->starting != head->end)
						result = NULL;
				}
			}
		}
	}
	else
	{
//checking the condition again, to make sure that the train can not enter the
//switch if it is in wrong direction
		for(count=1; count<MAX_SECTION;count++)
		{
			actualvalue = count;
			if((head->starting ==2 && main_head->starting == 3))
				actualvalue = 1;

			if((head->starting ==3 && main_head->starting == 2))
				actualvalue = 4;

		//	if(head->starting == 9 && main_head->starting == 10)
		//		actualvalue = 8;

			if(head->starting == 12 && main_head->starting == 11)
				actualvalue = 7;

			if(head->starting == 5 && main_head->starting == 6)
				actualvalue = 4;

			if((head->starting ==12 && main_head->starting == 5))
				actualvalue = 7;

			if((head->starting == 5 && main_head->starting == 12))
				actualvalue = 4;

			if((head->starting == 3 && main_head->starting == 8))
				actualvalue = 4;

			if((head->starting == 8 && main_head->starting == 3))
				actualvalue = 7;

			if(map_gate[start][actualvalue] == DIRECT)
			{
				if(head->section[actualvalue] == ENDING)
				{
					result = recursion(head,actualvalue,next_node);
					break;
				}
				else
				if(head->section[actualvalue] == AVAILABLE)
				{
					result = recursion(head,actualvalue,next_node);
					if(result != NULL)
					{
						if( result->starting == head->end)
							break;
						else
							result = NULL;
					}
				}
			}
			if(actualvalue != count)
				break;
		}
	}

	return result;
}

//locking the is used when the train is starting to enter the next segment,
//it is needed to lock the next segment in order the train can go in safely
//if between the segment having the switch, then turn the switch on and turn the segment addition on
//again: the direction is involved in here
unsigned int locking(unsigned int current_location, unsigned int next_location, unsigned int direction)
{
	switch(current_location)
	{
		case 1:
			if(direction)
			{
				semaWait((unsigned int *)&(railsystem[2].status));
				hall_sensor_dir_mag(2,DIR_SPEED(direction,railsystem[current_location].speed));
				if(next_location == 13)
				{
					//13
					semaWait((unsigned int *)&(railsystem[13].status));
					gate[1]=ON;
					hall_sensor_switch(1,0);
					hall_sensor_dir_mag(2,DIR_SPEED(direction,railsystem[current_location].speed));
				}
			}
			else
			{
				//6
				semaWait((unsigned int *)&(railsystem[6].status));
				hall_sensor_dir_mag(6,DIR_SPEED(direction,railsystem[current_location].speed));
			}
			break;
		case 2:
			if(direction)
			{
				if(next_location == 14)
				{
					//14
					semaWait((unsigned int *)&(railsystem[8].status));
					semaWait((unsigned int *)&(railsystem[14].status));
					railsystem[14].speed = railsystem[2].speed;
					railsystem[14].direction = railsystem[2].direction;
					hall_sensor_dir_mag(8,DIR_SPEED(direction,railsystem[current_location].speed));
					hall_sensor_dir_mag(14,DIR_SPEED(direction,railsystem[current_location].speed));
					gate[2]=ON;
					hall_sensor_switch(2,0);
				}
				else
				{
					//3
					semaWait((unsigned int *)&(railsystem[3].status));
					hall_sensor_dir_mag(3,DIR_SPEED(direction,railsystem[current_location].speed));
				}
			}
			else
			{
				//1
				semaWait((unsigned int *)&(railsystem[1].status));
				hall_sensor_dir_mag(1,DIR_SPEED(direction,railsystem[current_location].speed));
			}
			break;
		case 3:
			if(direction)
			{
				//4
				semaWait((unsigned int *)&(railsystem[4].status));
				hall_sensor_dir_mag(4,DIR_SPEED(direction,railsystem[current_location].speed));
			}
			else
			{
				if(next_location==2)
				{
					semaWait((unsigned int *)&(railsystem[2].status));
					hall_sensor_dir_mag(2,DIR_SPEED(direction,railsystem[current_location].speed));
				}
				else//8
				{
					semaWait((unsigned int *)&(railsystem[9].status));
					semaWait((unsigned int *)&(railsystem[8].status));
					gate[3]=ON;
					gate[7]=ON;
					hall_sensor_switch(3,0);
					hall_sensor_switch(7,0);
					update_the_next_value(6,16);
					hall_sensor_dir_mag(9,DIR_SPEED(direction,railsystem[current_location].speed));
					hall_sensor_dir_mag(8,DIR_SPEED(direction,railsystem[current_location].speed));
				}
			}
			break;
		case 4:
			if(direction)
			{
				if(next_location==5)
				{
					//5
					semaWait((unsigned int *)&(railsystem[5].status));
					hall_sensor_dir_mag(5,DIR_SPEED(direction,railsystem[current_location].speed));
				}
				else//16
				{
					semaWait((unsigned int *)&(railsystem[16].status));
					hall_sensor_dir_mag(16,DIR_SPEED(direction,railsystem[current_location].speed));
					gate[5]=ON;
					hall_sensor_switch(5,0);
				}
			}
			else
			{
				if(next_location==13)
				{
					semaWait((unsigned int *)&(railsystem[13].status));
					semaWait((unsigned int *)&(railsystem[3].status));
					gate[4]=ON;
					hall_sensor_switch(4,0);
					hall_sensor_dir_mag(3,DIR_SPEED(direction,railsystem[current_location].speed));
					hall_sensor_dir_mag(13,DIR_SPEED(direction,railsystem[current_location].speed));
				}
				else
				{//3
					semaWait((unsigned int *)&(railsystem[3].status));
					hall_sensor_dir_mag(3,DIR_SPEED(direction,railsystem[current_location].speed));
				}
			}
			break;
		case 5:
			if(direction)
			{
				if(next_location==12)
				{
					semaWait((unsigned int *)&(railsystem[12].status));
					semaWait((unsigned int *)&(railsystem[7].status));
					hall_sensor_dir_mag(12,DIR_SPEED(direction,railsystem[current_location].speed));
					hall_sensor_dir_mag(5,DIR_SPEED(direction,railsystem[current_location].speed));
					hall_sensor_dir_mag(7,DIR_SPEED(direction,railsystem[current_location].speed));
					gate[6]=ON;
					gate[8]=ON;
					hall_sensor_switch(6,0);
					hall_sensor_switch(8,0);
					copy_the_next_value(9,24);
				}
				else//6
				{
					semaWait((unsigned int *)&(railsystem[6].status));
					hall_sensor_dir_mag(6,DIR_SPEED(direction,railsystem[current_location].speed));
				}
			}
			else
			{
				//4
				semaWait((unsigned int *)&(railsystem[4].status));
				hall_sensor_dir_mag(4,DIR_SPEED(direction,railsystem[current_location].speed));
			}
			break;

		case 6:
			if(direction)
			{
				semaWait((unsigned int *)&(railsystem[1].status));
				hall_sensor_dir_mag(1,DIR_SPEED(direction,railsystem[current_location].speed));
			}
			else
			{
				semaWait((unsigned int *)&(railsystem[5].status));
				hall_sensor_dir_mag(7,DIR_SPEED(direction,railsystem[current_location].speed));
			}
			break;
		case 7:
			if(direction)
			{
				semaWait((unsigned int *)&(railsystem[8].status));
				semaWait((unsigned int *)&(railsystem[2].status));
				hall_sensor_dir_mag(8,DIR_SPEED(direction,railsystem[current_location].speed));
				hall_sensor_dir_mag(2,DIR_SPEED(direction,railsystem[current_location].speed));
			}
			else
			{
				semaWait((unsigned int *)&(railsystem[12].status));
				hall_sensor_dir_mag(12,DIR_SPEED(direction,railsystem[current_location].speed));
				if(next_location == 15)
				{
					semaWait((unsigned int *)&(railsystem[15].status));
					gate[9]=ON;
					hall_sensor_switch(9,0);
					hall_sensor_dir_mag(15,DIR_SPEED(direction,railsystem[current_location].speed));
				}
			}
			break;
		case 8:
			if(direction)
			{
				semaWait((unsigned int *)&(railsystem[9].status));
				hall_sensor_dir_mag(9,DIR_SPEED(direction,railsystem[current_location].speed));
				if(next_location == 3)
				{	//3
					semaWait((unsigned int *)&(railsystem[3].status));
					gate[7]=ON;
					gate[3]=ON;
					hall_sensor_switch(3,0);
					hall_sensor_switch(7,0);
					//hall_sensor_dir_mag(9,DIR_SPEED(direction,railsystem[current_location].speed));
					//hall_sensor_dir_mag(3,DIR_SPEED(direction,railsystem[current_location].speed));
					hall_sensor_dir_mag(8,DIR_SPEED(direction,railsystem[current_location].speed));
				}
			}
			else
			{
				//7
				semaWait((unsigned int *)&(railsystem[7].status));
				hall_sensor_dir_mag(7,DIR_SPEED(direction,railsystem[current_location].speed));
			}
			break;
		case 9:
			if(direction)
			{
				//10
				semaWait((unsigned int *)&(railsystem[10].status));
				hall_sensor_dir_mag(10,DIR_SPEED(direction,railsystem[current_location].speed));
			}
			else
			{
				//8
				semaWait((unsigned int *)&(railsystem[8].status));
				semaWait((unsigned int *)&(railsystem[2].status));
				hall_sensor_dir_mag(8,DIR_SPEED(direction,railsystem[current_location].speed));
				hall_sensor_dir_mag(2,DIR_SPEED(direction,railsystem[current_location].speed));
			}
			break;
		case 10:
			if(direction)
				{
					semaWait((unsigned int *)&(railsystem[11].status));
					hall_sensor_dir_mag(11,DIR_SPEED(direction,railsystem[current_location].speed));
				}
				else
				{
					semaWait((unsigned int *)&(railsystem[9].status));
					hall_sensor_dir_mag(9,DIR_SPEED(direction,railsystem[current_location].speed));
				}
				break;
		case 11:
			if(direction)
				{
					semaWait((unsigned int *)&(railsystem[12].status));
					semaWait((unsigned int *)&(railsystem[5].status));
					hall_sensor_dir_mag(12,DIR_SPEED(direction,railsystem[current_location].speed));
					hall_sensor_dir_mag(5,DIR_SPEED(direction,railsystem[current_location].speed));
				}
				else
				{
					semaWait((unsigned int *)&(railsystem[10].status));
					hall_sensor_dir_mag(10,DIR_SPEED(direction,railsystem[current_location].speed));
				}
				break;
		case 12:
			if(direction)
				{
					//7
					semaWait((unsigned int *)&(railsystem[7].status));
					hall_sensor_dir_mag(7,DIR_SPEED(direction,railsystem[current_location].speed));
				}
				else
				{
					semaWait((unsigned int *)&(railsystem[5].status));
					hall_sensor_dir_mag(5,DIR_SPEED(direction,railsystem[current_location].speed));
					if(next_location==5)
					{
						gate[8]=ON;
						gate[6]=ON;
						hall_sensor_switch(6,0);
						hall_sensor_switch(8,0);
						copy_the_next_value(24,9);
						semaWait((unsigned int *)&(railsystem[4].status));
						hall_sensor_dir_mag(4,DIR_SPEED(direction,railsystem[current_location].speed));
					}
					else
					{//11
						semaWait((unsigned int *)&(railsystem[11].status));
						hall_sensor_dir_mag(11,DIR_SPEED(direction,railsystem[current_location].speed));
					}
				}
				break;
		case 13:
			if(direction)
				{
					semaWait((unsigned int *)&(railsystem[3].status));
					semaWait((unsigned int *)&(railsystem[4].status));
					gate[4]=ON;
					hall_sensor_switch(4,0);
					hall_sensor_dir_mag(3,DIR_SPEED(direction,railsystem[current_location].speed));
					hall_sensor_dir_mag(4,DIR_SPEED(direction,railsystem[current_location].speed));
				}
				else
				{
					semaWait((unsigned int *)&(railsystem[2].status));
					semaWait((unsigned int *)&(railsystem[1].status));
					gate[1]=ON;
					hall_sensor_switch(1,0);
					hall_sensor_dir_mag(2,DIR_SPEED(direction,railsystem[current_location].speed));
					hall_sensor_dir_mag(1,DIR_SPEED(direction,railsystem[current_location].speed));
				}
				break;
		case 14:
			if(direction == CW)
			{
				semaWait((unsigned int *)&(railsystem[8].status));
				semaWait((unsigned int *)&(railsystem[2].status));
				railsystem[2].speed = railsystem[14].speed;
				railsystem[2].direction = railsystem[14].direction;
				hall_sensor_dir_mag(2,DIR_SPEED(direction,railsystem[current_location].speed));
				hall_sensor_dir_mag(8,DIR_SPEED(direction,railsystem[current_location].speed));
				gate[2]=ON;
				hall_sensor_switch(2,0);
			}
			break;
		case 15:
			if(direction)
			{
				semaWait((unsigned int *)&(railsystem[12].status));
				semaWait((unsigned int *)&(railsystem[7].status));
				gate[9] = ON;
				hall_sensor_switch(9,0);
				hall_sensor_dir_mag(12,DIR_SPEED(direction,railsystem[current_location].speed));
				hall_sensor_dir_mag(7,DIR_SPEED(direction,railsystem[current_location].speed));
			}
			break;
		case 16:
			if(direction)
			{
				semaWait((unsigned int *)&(railsystem[4].status));
				hall_sensor_dir_mag(4,DIR_SPEED(direction,railsystem[current_location].speed));
				gate[5] = ON;
				hall_sensor_switch(5,0);
			}
			break;
		default:
			break;
	}
	return FALSE;
}

//checking which segment is current segment
unsigned int get_current_section(unsigned int pid)
{
	unsigned int count;
	for(count=0;count<MAX_SECTION; count++)
	{
		if(railsystem[count].piD==pid)
			return count;
	}
	return 0;
}

//the train
//each train is supposed to run exactly this code
//looking for the new segment,
//determine the new segment to move
//write down all these information on railsystem
unsigned int Process(unsigned int data)
{
	unsigned int speed;
	unsigned int start, end;
	unsigned int pid;
	unsigned int old_section;
	struct TrainElement *temp;
	temp = (struct TrainElement *) data;
	start = temp->starting;
	end = temp->ending;
	speed = temp->speed;
	deallocate((unsigned int*)temp);
	//unsigned int temporary_direction;
	//unsigned int temporary_next_segment;
	unsigned int trigger;
	trigger = FALSE;
	printf("start: %d\nspeed: %d\nend: %d\n",start, speed, end);
	if(trigger==FALSE)
	{
		//pid = getID();
		pid = 2;
		railsystem[start].piD = pid;
		railsystem[start].speed = speed;
		railsystem[start].status = USED;
		railsystem[start].direction = CCW;

		trigger=TRUE;
	}

	printf("pid: %d speed: %d end: %d",railsystem[start].piD, railsystem[start].speed, end);

	while(1)
	{
		if(railsystem[get_current_section(pid)].next_segment==0)
		{
			old_section = start;
			//geting new segment
			start = get_current_section(pid);
			//temporary_direction = railsystem[start].direction;
			//temporary_next_segment = railsystem[start].next_segment;
			//if it is ending
			if(end==start)
			{
				if(start == 14 || start == 15 || start == 16)
					hall_sensor_dir_mag(start, DIR_SPEED(railsystem[start].direction,railsystem[start].speed));
				else
					railsystem[start].speed = 0;

				trigger = FALSE;
				printsectionavailable();
			}
			else
				railsystem[start].next_segment = making_decision(start, end, old_section);
			//process
			if(trigger)
			{
				//based on making_decision, it gives us the next segment
				//update the direction
				railsystem[start].direction = select_direction[start][railsystem[start].next_segment];
				//incase error occur
				if(start==0)
				{
					start = get_current_section(pid);
				}
				//locing the next segment
				//and turn the power on
				locking(start, railsystem[start].next_segment, railsystem[start].direction);
				//printf("\n\n<<%d %d %d %d>>\n\n", start, railsystem[start].direction, railsystem[start].speed, railsystem[start].next_segment);
				hall_sensor_dir_mag(start, DIR_SPEED(railsystem[start].direction,railsystem[start].speed));
				printsectionavailable();
			}
		}
		NVIC_INT_CTRL_R |= TRIGGER_PENDSV;
	}
	return TRUE;
}

