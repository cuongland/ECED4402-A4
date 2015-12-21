/*
 * ControlSoftware.c
 *
 *  Created on: Oct 14, 2015
 *      Author: Duc Cuong Dinh
 */
#include <stdio.h>
#include <stdlib.h>

#include "Queue.h"
#include "ControlSoftware.h"
#include "memory.h"
#include "uart.h"
#include "cursor.h"
#include "railsystem.h"
#include "Kernel.h"
#include "PcbQueue.h"
#include "train.h"

//#define testing_cs

//global variables
//buffer for transmit: the current transmitting
volatile char *TX;
//the buffer length of buffer_ready
volatile unsigned int length;

//command state:
volatile enum COMMAND_STATE current_state;

//other information:
//infmation for clock:
volatile unsigned int number1;
volatile unsigned int number2;
volatile unsigned int number3;

//how many char is pressed
volatile unsigned int char_input;

//inialization for Control Software
void initialization_Control_Software(void)
{
	TX  = NULL;
	current_state = VALID;
	number1 = 0;
	number2 = 0;
	number3 = 0;
	char_input = 15;
}

//get_received_char:
//when a character is press, the command and the char is stored in the queue
//when dequeueing the queue, the command and the data will be burst
//it is considered whether it is valid or invalid by sequence typing
//the invalid state will be reset when enter key is hit
unsigned int get_received_char(unsigned int data)
{
	char_input++;

	if(TX == NULL)
	{
		TX = (volatile char*) allocate(STRING_SIZE);
		length = 0;
	}
	switch(data)
	{
		case '\r':

			cursor_position(TX, &length,LINE_COMMAND,COLUMN);
			cursor_erase(TX, &length);
			if(current_state== END2)
				enqueue(get_init_value_train(number1, number2, number3),Process);
			char_input = 15;
			current_state = VALID;
			number1 = 0;
			number2 = 0;
			number3 = 0;
			break;
		case ' ':
			current_state = (current_state == START2) ? SPACE1 : ((current_state == SPEEDVALUE) ? SPACE2 : INVALID);
			break;
		case '0':
		case '1':
			switch(current_state)
			{
				case VALID:
					number1 = (data - '0') * BASE_NUMBER_10;
					current_state = START1;
					break;
				case START1:
					number1 = (data - '0') + number1;
					current_state = START2;
					break;
				case SPACE1:
					number2 = (data - '0');
					current_state = SPEEDVALUE;
					break;
				case SPACE2:
					number3 = (data - '0') * BASE_NUMBER_10;
					current_state = END1;
					break;
				case END1:
					number3 = (data - '0') + number3;
					current_state = END2;
					break;
				default:
					current_state = INVALID;
					break;
			}
			break;
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
			switch(current_state)
			{
				case START1:
					number1 = (data - '0') + number1;
					current_state = (number1 <MAX_SECTION && number1>0) ? START2: INVALID;
					break;
				case SPACE1:
					number2 = (data - '0');
					current_state = SPEEDVALUE;
					break;
				case END1:
					number3 = (data - '0') + number3;
					current_state = (number1 <MAX_SECTION && number1>0) ? END2: INVALID;
					break;
				default:
					current_state = INVALID;
					break;
			}
			break;
		case '8':
		case '9':
			switch(current_state)
			{
				case START1:
					number1 = (data - '0') + number1;
					current_state = (number1 <MAX_SECTION && number1>0) ? START2: INVALID;
					break;
				case END1:
					number3 = (data - '0') + number3;
					current_state = (number1 <MAX_SECTION && number1>0) ? END2: INVALID;
					break;
				default:
					current_state = INVALID;
					break;
			}
			break;
		default:
			current_state = INVALID;
			break;
	}
	generate_command_string(data);

	TX[length++] = (char)NULL_INDEX;//the end of string
	//printf("start: %d\nspeed: %d\nend: %d\n",number1, number2, number3);
	send_to_uart(1);

	return FALSE;
}

//generating the echo of typing
void generate_command_string(char c)
{
	if(TX != NULL)
	{
		//adding postion
		cursor_position(TX, &length, LINE_COMMAND, char_input);
		//if the char input is not enter key
		if(c != '\r')
		{
			//add char to the string
			TX[length++] = c;
		}
		else
			cursor_erase(TX, &length);
	}
}

//send the string to UART
//in the string should have cursor commands,
//and in the end, the char 0xFF (NULL_INDEX) is indecated it is the end of string
unsigned int send_to_uart(unsigned int data)
{
	TX[length++] = (char)NULL_INDEX;//the end of string
	send_to_UART(TX);
	TX = NULL;
	return FALSE;
}

unsigned int get_init_value_train(unsigned int num1, unsigned int num2, unsigned int num3)
{
	struct TrainElement *p;
	p = (struct TrainElement *)allocate(128);
	if(p==NULL)
		return FALSE;

	p->starting = num1;
	p->speed = num2;
	p->ending = num3;

	return (unsigned int) p;
}
