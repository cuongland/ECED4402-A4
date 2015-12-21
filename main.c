/*
 * main.c
 */
#include <stdio.h>
#include <stdlib.h>

#include "generalinfo.h"
#include "Queue.h"
#include "memory.h"
#include "PriorityQueue.h"
#include "uart.h"
#include "systick.h"
#include "ControlSoftware.h"
#include "PendSVHandler.h"
#include "cursor.h"
#include "Kernel.h"
#include "PcbQueue.h"

#include "spi.h"
#include "gpiof.h"
#include "messageatmel.h"
#include "messageuart.h"
#include "hallSensor.h"
#include "train.h"
#include "railsystem.h"
#include "testcase.h"
#include "switch.h"

extern unsigned int making_decision(unsigned int start, unsigned int end,unsigned int old_section);
extern volatile struct SectionElement railsystem[MAX_SECTION];

void init()
{
	getchar();
	initial_memory();
	initialize_Queue();
	InitializePriorityQueueManagement();
	initialization_uart();
	initialization_Control_Software();
	NVIC_SYS_PRI3_R |= PENDSV_LOWEST_PRIORITY;
	Configuration_and_Control |= BASETHR;

	//initialization_Control_Software();


	//RegistrationProcess(IldeQrocess,0);
	SPI_Init();
	PORTF_Init();

	unsigned int i;
	print_init_hall_sensor(1);
	for(i=1;i<=16;i++)
		hall_sensor_dir_mag(i,0);
	NVIC_INT_CTRL_R |= TRIGGER_PENDSV;
	for(i=1;i<10;i++)
		hall_sensor_switch(i,1);
	hall_sensor_queue_reset(atmel1);
	hall_sensor_queue_reset(atmel3);
	hall_sensor_reset_acknowledgement_all();

	NVIC_INT_CTRL_R |= TRIGGER_PENDSV;
	ENABLE_GLOBAL_INTERRUPT();

	getchar();
}

int main(void)
{
	init();

	unsigned int *temp = (unsigned int *)get_init_value_train(4,3,15);
	Process((unsigned int)temp);

	while(1);

	return 0;
}
