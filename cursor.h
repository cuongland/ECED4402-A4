/*
 * curcor.h
 *
 *  Created on: Oct 19, 2015
 *      Author: Duc Cuong Dinh
 */

#ifndef CURCOR_H_
#define CURCOR_H_

#define MAX_ROW				24
#define MAX_COL				80
#define CURSOR_COMMAND_POSITION_SIZE	9
#define CURSOR_COMMAND_ERASE_SIZE		5
#define CURSOR_COMMAND_SAVE_SIZE		4
#define CURSOR_COMMAND_RESTORE_SIZE		3
#define ROW_HIGH_BYTE		7
#define ROW_LOW_BYTE		6
#define COL_HIGH_BYTE		4
#define COL_LOW_BYTE		3

//cursor_position: update destination string with cursor command and update locations
void cursor_position(volatile char *string, volatile unsigned int *location, unsigned int row, unsigned int col);

//add erase command to destination string
void cursor_erase(volatile char *string, volatile unsigned int *location);

//add save position cursor to destination string
void cursor_save(volatile char *string, volatile unsigned int *location);

//add restore cursor position to destination
void cursor_restore(volatile char *string, volatile unsigned int *location);

//cursor_command: duplicating char from the source to destination string
//input: address of destination string, address of source, index of destination, size of string
void cursor_command(volatile char *destination, volatile char *source, volatile unsigned int **location, unsigned int s);


#endif /* CURCOR_H_ */
