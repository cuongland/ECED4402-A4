/*
 * cursor.c
 * copy cursor command to destination command.
 *
 *  Created on: Oct 19, 2015
 *      Author: Duc Cuong Dinh
 *
 */

#include <stdio.h>
#include "cursor.h"
#include "generalinfo.h"

// Define and initialize an escape sequence for cursor position
volatile char cursor_command_position[] = {ESC, '[', '0', '0', ';', '0', '0', 'H', '\0'};
volatile char cursor_command_erase[]    = {ESC, '[', 'K', '\0'};
volatile char cursor_command_save[]		= {ESC, '[', 's', '\0'};
volatile char cursor_command_restore[]	= {ESC, '8', '\0'};

//cursor_command: duplicating char from the source to destination string
//input: address of destination string, address of source, index of destination, size of string
void cursor_command(volatile char *destination, volatile char *source, volatile unsigned int **location, unsigned int size)
{
	//copy the index to count_destination so that the system does not have to calculate number of times
	unsigned int count_destination = (**location);
	unsigned int count_source;
	//copy characters
	for(count_source =0; count_source<size; count_source++,count_destination++)
		destination[count_destination] = source[count_source];
	//storing back to location
	(**location) = count_destination;
}

//cursor_position: update destination string with cursor command and update locations
void cursor_position(volatile char *string, volatile unsigned int *location, unsigned int row, unsigned int col)
{
	//add position command to the destination string
	cursor_command(string, cursor_command_position, &location, CURSOR_COMMAND_POSITION_SIZE);
	//update the position
	//row
	if(row <= MAX_ROW)
	{
		string[(*location) - ROW_HIGH_BYTE] = row / BASE_NUMBER_10 + '0';
		string[(*location) - ROW_LOW_BYTE]  = row % BASE_NUMBER_10 + '0';
	}
	//columne
	if(col <= MAX_COL)
	{
		string[(*location) - COL_HIGH_BYTE] = col / BASE_NUMBER_10 + '0';
		string[(*location) - COL_LOW_BYTE]  = col % BASE_NUMBER_10 + '0';
	}
}

//add erase command to destination string
void cursor_erase(volatile char *string, volatile unsigned int *location)
{
	cursor_command(string, cursor_command_erase, &location, CURSOR_COMMAND_ERASE_SIZE);
}

//add save position cursor to destination string
void cursor_save(volatile char *string, volatile unsigned int *location)
{
	cursor_command(string, cursor_command_save, &location, CURSOR_COMMAND_SAVE_SIZE);
}

//add restore cursor position to destination
void cursor_restore(volatile char *string, volatile unsigned int *location)
{
	cursor_command(string, cursor_command_restore, &location, CURSOR_COMMAND_RESTORE_SIZE);
}
