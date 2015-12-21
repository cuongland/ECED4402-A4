/*
 * messageatmel.h
 *
 *  Created on: Dec 5, 2015
 *      Author: Duc Cuong Dinh
 */

#ifndef MESSAGEATMEL_H_
#define MESSAGEATMEL_H_

#define COMMANDFRAME	6
#define atmel1			1
#define atmel2			2
#define atmel3			3

#define SENT_B_1		0xAA
#define SENT_B_5		0x55
#define SENT_B_6		0xFF

#define REC_B_1			0xFF
#define REC_B_2			0xCC
#define	REC_B_6_S		0x99
#define	REC_B_6_F		0x3F

#define HALL_SENT_B_2_I	0xA0
#define HALL_SENT_B_2_R	0xAE
#define HALL_SENT_B_3	0xFF
#define HALL_SENT_B_4	0xFF

#define HALL_REC_B_3_I	0xA0
#define HALL_REC_B_3_R	0xAE
#define	HALL_REC_B_5	0xFF

#define DM_SENT_2_S		0xB0
#define DM_REC_3_S		0xB0

#define DM_SENT_2_A		0xBE
#define DM_SENT_3_A		0xFF
#define	DM_REC_3_A		0xBE
#define	DM_REC_4_A		0xFF

#define HALL_ACK_SENT_2	0xC0
#define HALL_ACK_SENT_4	0xFF
#define HALL_ACK_REC_3	0xC0
#define	HALL_ACK_REC_5	0xFF

#define HALL_RES_SENT_2	0xCE
#define HALL_RES_SENT_3 0xFF
#define HALL_RES_SENT_4 0xFF
#define HALL_RES_REC_3	0xCE
#define HALL_RES_REC_4 	0xFF
#define HALL_RES_REC_5 	0xFF

#define SW_SENT_2		0xD0
#define SW_REC_3		0xD0

struct CommandFrame
{
	unsigned char b1;
	unsigned char b2;
	unsigned char b3;
	unsigned char b4;
	unsigned char b5;
	unsigned char b6;
};

struct CommandPacket
{
	unsigned int atmelnumber;
	char *sending;
	char *receiving;
};

unsigned int hall_sensor_input(unsigned int atmelnumber);
unsigned int hall_sensor_queue_reset(unsigned int atmelnumber);
unsigned int hall_sensor_reset_acknowledgement(unsigned char hallsensornumber);
unsigned int hall_sensor_reset_acknowledgement_all(void);
unsigned int hall_sensor_switch(unsigned char switchnumber, unsigned char direction);
unsigned int hall_sensor_dir_mag(unsigned int segment, unsigned int spped_dir);
unsigned int hall_sensor_dir_mag_all(unsigned int spped_dir);
unsigned int hall_sensor_command(unsigned int data);
unsigned int create_string_to_show_hall_sensor(unsigned int number);
void hall_sensor_print_to_UART(unsigned int data);
#endif /* MESSAGEATMEL_H_ */
