/*
 * messageuart.c
 *
 *  Created on: Dec 8, 2015
 *      Author: Duc Cuong Dinh
 */
#include <stdio.h>

#include "generalinfo.h"
#include "memory.h"
#include "uart.h"
#include "cursor.h"
#include "switch.h"
extern volatile enum SWITCH_STATE gate[MAX_SWITCH];

unsigned int print_init_hall_sensor(unsigned int data)
{
	char *string;
	unsigned int location;

	string = (char*) allocate(STRING_SIZE);
	if(string==NULL)
		return FALSE;
	location = 0;
	cursor_position (string, &location, 1, 1);

	string[location++] = 'h';
	string[location++] = 'a';
	string[location++] = 'l';
	string[location++] = 'l';
	string[location++] = ' ';
	string[location++] = 's';
	string[location++] = 'e';
	string[location++] = 'n';
	string[location++] = 's';
	string[location++] = 'o';
	string[location++] = 'r';
	string[location++] = ':';
	string[location++] = ' ';
	string[location++] = (char)NULL_INDEX;//the end of string
	send_to_UART(string);
	string = NULL;
	return TRUE;
}

unsigned int print_hall_sensor(unsigned int number)
{
	char *string;
	unsigned int location;

	string = (char*) allocate(STRING_SIZE);
	if(string==NULL)
		return FALSE;
	location = 0;
	cursor_position (string, &location, 1, 14);
	string[location++] = (number/10)+'0';
	string[location++] = (number%10)+'0';
	string[location++] = (char)NULL_INDEX;//the end of string
	send_to_UART(string);
	string = NULL;
	return TRUE;
}

unsigned int print_switch(unsigned int switchnumber)
{
	char *switch_result;
	unsigned int location;
	switch_result = (char *)allocate(STRING_SIZE);

	if(switch_result == NULL)
		return FALSE;

	location = 0;
	cursor_position(switch_result,&location,switchnumber*6,1);
	if(gate[switchnumber]==ON)
	{
		switch_result[location++] = switchnumber + '0';
		switch_result[location++] = ':';
		switch_result[location++] = 'O';
		switch_result[location++] = 'N';
		switch_result[location++] = ' ';
	}
	else
	{
		switch_result[location++] = switchnumber + '0';
		switch_result[location++] = ':';
		switch_result[location++] = 'O';
		switch_result[location++] = 'F';
		switch_result[location++] = 'F';
	}
	switch_result[location++] = (char)NULL_INDEX;//the end of string
	send_to_UART(switch_result);
	switch_result = NULL;
	return TRUE;
}
