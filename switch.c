/*
 * switch.c
 *
 *  Created on: Dec 2, 2015
 *      Author: Duc Cuong Dinh
 */

#include "switch.h"

volatile enum SWITCH_STATE gate[MAX_SWITCH] = {INV,OFF,OFF,OFF,OFF,
											   OFF,OFF,OFF,OFF,OFF};

