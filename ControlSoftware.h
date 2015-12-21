/*
 * ControlSoftware.h
 *
 *  Created on: Oct 14, 2015
 *      Author: Duc Cuong Dinh
 */

#ifndef CONTROLSOFTWARE_H_
#define CONTROLSOFTWARE_H_

#define LINE_COMMAND		2
#define LINE_CLOCK			1
#define COLUMN				15

enum COMMAND_STATE {INVALID, VALID,
					START1, START2, SPACE1,
					SPEEDVALUE, SPACE2,
					END1, END2
					};

void initialization_Control_Software(void);
unsigned int get_received_char(unsigned int data);
void generate_command_string(char c);
unsigned int send_to_uart(unsigned int data);
unsigned int get_init_value_train(unsigned int num1, unsigned int num2, unsigned int num3);
#endif /* CONTROLSOFTWARE_H_ */
