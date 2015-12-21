/*
 * messageatmel.c
 *
 *  Created on: Dec 5, 2015
 *      Author: Duc Cuong Dinh
 */

#include <stdio.h>
#include "generalinfo.h"
#include "memory.h"
#include "messageatmel.h"
#include "spi.h"
#include "Queue.h"
#include "cursor.h"
#include "uart.h"
#include "HallSensor.h"
#include "messageuart.h"
#include "semaphore.h"
#include "railsystem.h"
#include "switch.h"

extern volatile struct SectionElement railsystem[MAX_SECTION];
extern volatile enum SWITCH_STATE gate[MAX_SWITCH];


unsigned int command_frame(unsigned int atmelnumber, unsigned char ds2,
									unsigned char ds3, unsigned char ds4)
{
	struct CommandFrame *cf_s;
	struct CommandFrame *cf_r;
	struct CommandPacket *packet;

	cf_r = (struct CommandFrame *)allocate(COMMANDFRAME);
	if(cf_r==NULL)
		return FALSE;
	cf_s = (struct CommandFrame *)allocate(COMMANDFRAME);
	if(cf_s==NULL)
		return FALSE;
	packet = (struct CommandPacket *)allocate(COMMANDFRAME);
	if(packet==NULL)
		return FALSE;

	//setup
	cf_s->b1 = SENT_B_1;
	cf_s->b2 = ds2;
	cf_s->b3 = ds3;
	cf_s->b4 = ds4;
	cf_s->b5 = SENT_B_5;
	cf_s->b6 = SENT_B_6;

	packet->atmelnumber = atmelnumber;
	packet->sending = (char *) cf_s;
	packet->receiving = (char *) cf_r;

	enqueue((unsigned int)packet, hall_sensor_command);

	cf_s = NULL;
	cf_r = NULL;

	return TRUE;
}

unsigned int hall_sensor_input(unsigned int atmelnumber)
{
	command_frame(atmelnumber,HALL_SENT_B_2_I,HALL_SENT_B_3,HALL_SENT_B_4);
	return TRUE;
}

unsigned int hall_sensor_queue_reset(unsigned int atmelnumber)
{
	command_frame(atmelnumber,HALL_SENT_B_2_R,HALL_SENT_B_3,HALL_SENT_B_4);
	return TRUE;
}

unsigned int hall_sensor_reset_acknowledgement(unsigned char hallsensornumber)
{
	command_frame( 1 ,HALL_ACK_SENT_2,hallsensornumber,HALL_ACK_SENT_4);
	obtain_decision(hallsensornumber);
	print_hall_sensor(hallsensornumber);
	printf("<<%d>>\n",hallsensornumber);
	return FALSE;
}

unsigned int hall_sensor_reset_acknowledgement_all(void)
{
	command_frame( atmel1 ,HALL_RES_SENT_2,HALL_RES_SENT_3,HALL_RES_SENT_4);
	return TRUE;
}

unsigned int hall_sensor_switch(unsigned char switchnumber, unsigned char direction)
{
	command_frame( atmel2 ,SW_SENT_2,switchnumber,direction);
	return TRUE;
}

unsigned int hall_sensor_dir_mag(unsigned int segment, unsigned int spped_dir)
{
	command_frame( atmel1 ,DM_SENT_2_S,segment,spped_dir);
	if(spped_dir)
		semaPost((unsigned int*)&(railsystem[segment].status));
	//printf("dir_mag: %d-%d", segment, spped_dir);
	return FALSE;
}

unsigned int hall_sensor_dir_mag_all(unsigned int spped_dir)
{
	command_frame( atmel1 ,DM_SENT_2_A,DM_SENT_3_A,spped_dir);
	return TRUE;
}

unsigned int hall_sensor_command(unsigned int data)
{
	struct CommandPacket *packet = (struct CommandPacket *) data;
	char *send = packet->sending;
	char *receive = packet->receiving;
	unsigned int atmel = packet->atmelnumber;
	unsigned int count;

	do
	{
		for(count=0; count<COMMANDFRAME; count++)
		{
			SPIDataTransmit(send[count],atmel);
			WaitSPIBusy();
			receive[count] = SPIDataReceive();
		}
	}
	while(receive[COMMANDFRAME-1] == REC_B_6_F);

	if(receive[2]==HALL_REC_B_3_I)
	{
		hall_sensor_reset_acknowledgement(receive[3]);
	}
	deallocate((unsigned int *)send);
	deallocate((unsigned int *)receive);
	deallocate((unsigned int *)packet);

	return FALSE;
}

void hall_sensor_print_to_UART(unsigned int data)
{
	char *s = (char *)allocate(128);
	char *t = (char *) data;
	unsigned int l = 0;
	unsigned int k = 0;
	cursor_position(s,&l,21,1);
	for(k=0;k<6;k++,l++)
		s[l] = t[k];
	s[l++] = 0xFF;
	send_to_UART(s);
}
