/*
 * FILE: memory.h
 *
 *  Created on: Sep 25, 2015
 *      Author: Duc Cuong Dinh
 *      		Jie Zhange
 */

#ifndef MEMORY_H_
#define MEMORY_H_

//for memory

#define MAX_MEM_MANAGEMENT 	248
#define MAX_MEM_CATEGORIES	5
//for level
#define LVL128				128
#define LVL64   			64
#define LVL32				32
#define LVL16				16
#define LVL8				8
#define LVL_SIZE_128		0x80
#define LVL_SIZE_64   		0x100
#define LVL_SIZE_32			0x200
#define LVL_SIZE_16			0x400
#define LVL_SIZE_8			0x800
#define LVL_INDEX_128		0
#define LVL_INDEX_64		128
#define LVL_INDEX_32		192
#define LVL_INDEX_16		224
#define LVL_INDEX_8			240

#define ADDDRESS_BASE		0x20004000

/*
 *  enum
 */

// this is
enum LEVELS {LEVEL128=0, LEVEL64=1, LEVEL32=2, LEVEL16=3, LEVEL8=4};

/*
 *  macro functions
 */
//is_free is return TRUE(1) or FALSE(0) based on the bit 0
#define IS_FREE(x)			(0x1 & (x))

//set_free: is set bit 0 to ONE
#define SET_FREE(x)			( 0x1 | (x) )

//set_not_free: is set bit 0 to ZERO
#define SET_NOT_FREE(x)		( 0xFFFFFFFE & (x) )

//set_level: level of the index will be at the bit 1,2,3, the value will be 0 to 4
//(x, y):
#define SET_LEVEL(x, y)		( (x) | ( (y) << 1 ) )

//get_level: getting level in the value
#define GET_LEVEL(x)		((0xE & (x)) >> 1)

//set_index: setup the next INDEX is available(free)/linking to next node
//(x, y): x is a value holding index,
//		  y is a target index to be stored in x
#define SET_INDEX(x, y)		((0xFFFFF00F & (x) ) | ( (y) << 4 ) )

//get_index:
#define GET_INDEX(x)		(((x) & 0xFF0) >> 4)

//set_index_NULL: setup the next INDEX is NULL (0xFF), the end of the linked list
#define SET_INDEX_NULL(x)	( (x) | 0xFF0)

//
#define SET_OFFSET(x, y)	(( (y) << 12) | (x) )
//
#define GET_OFFSET(x)		( (x) >>12)

//get_current_available: getting the current available
#define GET_CURRENT_AVAILABLE(x)	( (x) & 0xFF )

//set_current_available(x, y)
#define SET_CURRENT_AVAILABLE(x, y)	 (( (x) & 0xFFFFFF00) | (y) )

//last_available: getting the last available
#define GET_LAST_AVAILABLE(x)		(( (x) & 0XFF00) >> 8)

//
#define SET_LAST_AVAILABLE(x, y)	(( (x) & 0xFFFF00FF) | ( (y)<<8))

//current_last: merge current and last to single value in 16 bits
#define SET_CURRENT_LAST(x, y)		( (x) | ( (y) <<8))

//allocating function
//MASK_SIZE: is used to mask the size that is inputted in allocating function
#define MASK_SIZE(x)			 ( (x) >>7)

//deallocating
//mask_deallocationg_level:
#define MASK_DEALLOCATING_LEVEL(x)	((x)>>14)
//mask_deallocating_level0:
//#define MASK_DEALLOCATING_LEVEL_0_LOWER_BOUND(X)	((X) &  0x7F)
#define MASK_DEALLOCATING_LEVEL_0_HIGHER_BOUND(X)	((X) >> 7)
//mask_deallocating_level0:
//#define MASK_DEALLOCATING_LEVEL_1_LOWER_BOUND(X)	((X) &  0xFF)
#define MASK_DEALLOCATING_LEVEL_1_HIGHER_BOUND(X)	(((X) >> 8) & 0x3F)
//mask_deallocating_level0:
//#define MASK_DEALLOCATING_LEVEL_2_LOWER_BOUND(X)	((X) &  0x1FF)
#define MASK_DEALLOCATING_LEVEL_2_HIGHER_BOUND(X)	(((X) >> 9) & 0x1F)
//mask_deallocating_level0:
//#define MASK_DEALLOCATING_LEVEL_3_LOWER_BOUND(X)	((X) &  0x2FF)
#define MASK_DEALLOCATING_LEVEL_3_HIGHER_BOUND(X)	(((X) >> 10) & 0xF)
//mask_deallocating_level0:
//#define MASK_DEALLOCATING_LEVEL_4_LOWER_BOUND(X)	((X) &  0x3FF)
#define MASK_DEALLOCATING_LEVEL_4_HIGHER_BOUND(X)	(((X) >> 11) & 0x7)

//functions
void initial_memory(void);
void print_info_memory(void);
unsigned int * allocate(unsigned int size);
unsigned int deallocate(unsigned int * address);

#endif /* MEMORY_H_ */
