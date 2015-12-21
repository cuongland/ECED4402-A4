/*
 *  File: memory.c
 *
 *  The file is used to allocate memory to cpu or program
 *                      deallocate memory when cpu or program want to get free memory
 *
 *  Created on: Sep 25, 2015
 * 	Authors: Duc Cuong Dinh (B00516935)
 * 	         Jie Zhang      (B00583787)
 */
#include <stdio.h>
#include <stdlib.h>

#include "memory.h"
#include "generalinfo.h"

/*
 * for testing
 */
//#define testing
//#define testing_level

/*
 *  global variable
 */
volatile unsigned int memory_management[MAX_MEM_MANAGEMENT];
volatile unsigned int memory_control[MAX_MEM_CATEGORIES];
//extern	void*	_sys_memory;							 //	Start	of	system	memory
//extern	int	__SYSMEM_SIZE;			 				//	Size	of	system	memory

//initial value for memory management and control
//at the begining, every slot is available
//the current available is at the begin of each level
//the last available is at the end of each level
void initial_memory(void)
{
	unsigned int i,j,current,last,offset;
	i=0;

	for(j=0,current = 0,offset=0; j<LVL128; j++,i++)
	{
		memory_management[i] = SET_FREE(memory_management[i]);
		memory_management[i] = SET_LEVEL((memory_management[i]), LEVEL128);
		memory_management[i] = SET_INDEX((memory_management[i]), (i + 1));
		memory_management[i] = SET_OFFSET(memory_management[i], offset);
		//printf("%d: %d\n",i,deallocate(offset));
		offset += LVL_SIZE_128;
	}
	memory_management[i-1] = SET_INDEX_NULL(memory_management[i-1]);
	last = i-1;
	memory_control[LEVEL128] = SET_CURRENT_LAST(current, last);

	for(j=0, current = i;j<LVL64;j++,i++)
	{
		memory_management[i] = SET_FREE(memory_management[i]);
		memory_management[i] = SET_LEVEL((memory_management[i]), LEVEL64);
		memory_management[i] = SET_INDEX((memory_management[i]), (i + 1));
		memory_management[i] = SET_OFFSET(memory_management[i], offset);
		//printf("%d: %d\n",i,deallocate(offset));
		offset += LVL_SIZE_64;
	}
	memory_management[i-1] = SET_INDEX_NULL(memory_management[i-1]);
	last = i-1;
	memory_control[LEVEL64] = SET_CURRENT_LAST(current, last);

	for(j=0, current = i;j<LVL32;j++,i++)
	{
		memory_management[i] = SET_FREE(memory_management[i]);
		memory_management[i] = SET_LEVEL((memory_management[i]), LEVEL32);
		memory_management[i] = SET_INDEX((memory_management[i]), (i + 1));
		memory_management[i] = SET_OFFSET(memory_management[i], offset);
		//printf("%d: %d\n",i,deallocate(offset));
		offset += LVL_SIZE_32;
	}
	memory_management[i-1] = SET_INDEX_NULL(memory_management[i-1]);
	last = i-1;
	memory_control[LEVEL32] = SET_CURRENT_LAST(current, last);

	for(j=0, current = i;j<LVL16;j++,i++)
	{
		memory_management[i] = SET_FREE(memory_management[i]);
		memory_management[i] = SET_LEVEL((memory_management[i]), LEVEL16);
		memory_management[i] = SET_INDEX((memory_management[i]), (i + 1));
		memory_management[i] = SET_OFFSET(memory_management[i], offset);
		//printf("%d: %d\n",i,deallocate(offset));
		offset += LVL_SIZE_16;
	}
	memory_management[i-1] = SET_INDEX_NULL(memory_management[i-1]);
	last = i-1;
	memory_control[LEVEL16] = SET_CURRENT_LAST(current, last);

	for(j=0, current = i;j<LVL8;j++,i++)
	{
		memory_management[i] = SET_FREE(memory_management[i]);
		memory_management[i] = SET_LEVEL((memory_management[i]), LEVEL8);
		memory_management[i] = SET_INDEX((memory_management[i]), (i + 1));
		memory_management[i] = SET_OFFSET(memory_management[i], offset);
		//printf("%d: %d\n",i,deallocate(offset));
		offset += LVL_SIZE_8;
	}
	memory_management[i-1] = SET_INDEX_NULL(memory_management[i-1]);
	last = i-1;
	memory_control[LEVEL8] = SET_CURRENT_LAST(current, last);
}

void print_info_memory(void)
{
	unsigned int i;
	printf("memory management:\n");
	for(i=0; i<MAX_MEM_MANAGEMENT;i++)
		printf("index %d: %08x\n", i, memory_management[i]);
	printf("memory control:\n");
	for(i=0;i<MAX_MEM_CATEGORIES;i++)
		printf("index %d: %08x\n", i, memory_control[i]);
}

/*
 * The allocation routine should attempt to return the address of a memory block that is at least as long as the specified
*  size; in the worst case, this means returning a 2 KiB block for a one byte request. If the size request cannot be met (i.e.,
*  no memory available or the request is greater than 2 KiB), an address of NULL is to be returned. It is up to the application
*  to recognize the NULL pointer and terminate its activities; for example:
*  			char *ptr; ...
* 			ptr = allocate(512);
* 			if (ptr == NULL)
* 			{
* 			printf("Allocation failed\n");
* 			return;
* 			}
 */
unsigned int * allocate(unsigned int size)
{
	unsigned int level;
	unsigned int error;
	unsigned int current_available;
	unsigned int temp_memory_management;
	unsigned int temp_memory_control;
	unsigned int address;
	address = (unsigned int)NULL;
	error = FALSE;

	//step 1: determine the size is the best fit
#ifdef testing
	printf("step1: size %d , %x and masked %d , %x", size, size, MASK_SIZE(size), MASK_SIZE(size));
#endif

	switch (MASK_SIZE((size-1)))
	{
		//level 0
			case 0:
				level  = LEVEL128;
				//step 2: checking the level whether any slopts are available by checking the current available
				temp_memory_control = memory_control[level];
				current_available = GET_CURRENT_AVAILABLE(temp_memory_control);
				if(current_available!= NULL_INDEX)
					break;
				/* no break */
		//level 1
			case 1:
				level = LEVEL64;
				//step 2: checking the level whether any slopts are available by checking the current available
				temp_memory_control = memory_control[level];
				current_available = GET_CURRENT_AVAILABLE(temp_memory_control);
				if(current_available!= NULL_INDEX)
					break;
				/* no break */
		//level 2:
			case 2:
			case 3:
				level = LEVEL32;
				//step 2: checking the level whether any slopts are available by checking the current available
				temp_memory_control = memory_control[level];
				current_available = GET_CURRENT_AVAILABLE(temp_memory_control);
				if(current_available!= NULL_INDEX)
					break;
				/* no break */
		//level 3
			case 4:
			case 5:
			case 6:
			case 7:
				level = LEVEL16;
				//step 2: checking the level whether any slopts are available by checking the current available
				temp_memory_control = memory_control[level];
				current_available = GET_CURRENT_AVAILABLE(temp_memory_control);
				if(current_available!= NULL_INDEX)
					break;
				/* no break */
		//level 4
			case 8:
			case 9:
			case 10:
			case 11:
			case 12:
			case 13:
			case 14:
			case 15:
				level = LEVEL8;
				//step 2: checking the level whether any slopts are available by checking the current available
				temp_memory_control = memory_control[level];
				current_available = GET_CURRENT_AVAILABLE(temp_memory_control);
				if(current_available!= NULL_INDEX)
					break;
				/* no break */
		//any other case will be error
			default:
				error = TRUE;
				break;
	}

#ifdef testing_level
	printf("<level, value>: < %d, %d> \n", level, size);
#endif

	if (error == FALSE)
	{
		//level is valid
		//the value is 0,1,2,3,4
		//the slot is still available
		//step3: change the status from FREE to NOT FREE
		temp_memory_management = memory_management[current_available];
		temp_memory_management = SET_NOT_FREE(temp_memory_management);

		//step4: getting offset
		address = GET_OFFSET(temp_memory_management) + ADDDRESS_BASE;

		//step5: save back to memory_management
		memory_management[current_available] = temp_memory_management;

		//step6: change the current available to the next one
		current_available = GET_INDEX(temp_memory_management);

		//step7:save current available back to memory control
		temp_memory_control = SET_CURRENT_AVAILABLE(temp_memory_control, current_available);
		memory_control[level] = temp_memory_control;
	}
	//printf("enqueue: %08x\n",address);
	return (unsigned int *)address;
}


/*
 * The deallocation routine	expects	an address of a	memory block; the address must match the
*  location	of one of the blocks currently allocated. If the address is invalid or the block is not in
*  use,	the deallocation routine should return a value of FALSE; otherwise the block is available for
*  reuse and a value of TRUE is to be returned;	for	example:
*
*	if	(!deallocate(ptr))
*	{
*		printf("Deallocation	failed\n");
* 		return;
*	}
*/

/* function name: deallocate(pointer of physical address)
 * parameter is a pointer
 * return is TRUE or FALSE
 *
 * step 1: determine a pointer to index of the array
 * step 2: update the array
 */

unsigned int deallocate(unsigned int * address)
{
	//varailable
	unsigned int result;		//the result for returning of the function in the end
	unsigned int target_index;
	unsigned int temp_target_memory_management;
	unsigned int temp_memory_control;
	unsigned int temp_level;
	unsigned int temp_current;
	unsigned int temp_last;
	unsigned int temp_address;

	//initializing value
	result = TRUE;
	target_index = NULL_INDEX;
	temp_address = ((unsigned int) address) - ADDDRESS_BASE;

	//step1: checking the address is valid or not
	//		 if it is valid, calculate the level and index in the memory_management
	switch(MASK_DEALLOCATING_LEVEL(temp_address))
	{
		case 0:
			//level 0
			//if(MASK_DEALLOCATING_LEVEL_0_LOWER_BOUND(temp_address))
			//	result = FALSE;
			//else
			target_index = MASK_DEALLOCATING_LEVEL_0_HIGHER_BOUND(temp_address);
			break;
		case 1:
			//level 1
			//if(MASK_DEALLOCATING_LEVEL_1_LOWER_BOUND(temp_address))
			//	result = FALSE;
			//else
			target_index = LVL_INDEX_64 + MASK_DEALLOCATING_LEVEL_1_HIGHER_BOUND(temp_address);
			break;
		case 2:
			//level 2
			//if(MASK_DEALLOCATING_LEVEL_2_LOWER_BOUND(temp_address))
			//	result = FALSE;
			//else
			target_index = LVL_INDEX_32 + MASK_DEALLOCATING_LEVEL_2_HIGHER_BOUND(temp_address);
			break;
		case 3:
			//level 3
			//if(MASK_DEALLOCATING_LEVEL_3_LOWER_BOUND(temp_address))
			//	result = FALSE;
			//else
			target_index = LVL_INDEX_16 + MASK_DEALLOCATING_LEVEL_3_HIGHER_BOUND(temp_address);
			break;
		case 4:
			//level 4
			//if(MASK_DEALLOCATING_LEVEL_4_LOWER_BOUND(temp_address))
			//	result = FALSE;
			//else
			target_index = LVL_INDEX_8 + MASK_DEALLOCATING_LEVEL_4_HIGHER_BOUND(temp_address);
			break;
		default:
			//printf("default: %d - %d\n",MASK_DEALLOCATING_LEVEL(temp_address), temp_address);
			result = FALSE;
			break;
	}

	//when the address is a valid
	//the target_index is known
	if(result)
	{
		//getting value
		temp_target_memory_management = memory_management[target_index];
		//if the target index is still free, then the deallocate the address is error
		if(IS_FREE(temp_target_memory_management))
			result = FALSE;
		else
		{
			//the location is not free
			//getting level in memory management
			temp_level = GET_LEVEL(temp_target_memory_management);

			//getting the index of current and last
			temp_memory_control = memory_control[temp_level];
			temp_current = GET_CURRENT_AVAILABLE(temp_memory_control);
			temp_last = GET_LAST_AVAILABLE(temp_memory_control);

#ifdef testing
			printf("level:%x control:%x current:%x last:%x\n",temp_level, temp_memory_control, temp_current, temp_last);
#endif

			//when current available is point to NULL,
			// it mean the whole section is in used
			if(temp_current == NULL_INDEX)
				//setting current and last point to the new block
				temp_memory_control = SET_CURRENT_LAST(target_index, target_index);
			else
			{
				//when current available does not point to NULL
				//update memory control
				temp_memory_control = SET_LAST_AVAILABLE(temp_memory_control, target_index);
				//update the last available,
				//now it point to the new one
				memory_management[temp_last] = SET_INDEX(memory_management[temp_last], target_index);
			}

			//update the status of the target index
			//a bit: is Free at bit 0
			temp_target_memory_management = SET_FREE(temp_target_memory_management);
			//a next index: bit from 4 to 11
			temp_target_memory_management = SET_INDEX_NULL(temp_target_memory_management);

			//update back to the array
			memory_control[temp_level] = temp_memory_control;
			memory_management[target_index] = temp_target_memory_management;
		}
	}

	//return
	return result;
}
