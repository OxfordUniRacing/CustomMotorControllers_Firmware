/*
 * PositionSensors.h
 *
 * Created: 25/02/2021 11:21:45
 *  Author: Ruzhev
 */ 

/*

This file contains all necessary functions for interfacing with the position sensors

*/

#ifndef POSITIONSENSORS_H_
#define POSITIONSENSORS_H_

#include "User_Config.h"

/*
From motor datasheet
					Step1	Step2	Step3	Step4	Step5	Step6	Error1	Error2
Phase A(U)		->	+		Z		-		-		Z		+		Z		Z
Phase B(V)		->	Z		+		+		Z		-		-		Z		Z
Phase C(W)		->	-		-		Z		+		+		Z		Z		Z
Sensor A (POS1)	->	1		1		0		0		0		1		0		1
Sensor B (POS2)	->	0		1		1		1		0		0		0		1
Sensor C (POS3)	->	0		0		0		1		1		1		0		1

Position convention:
Step 1 <=> first 60 degree sector
Step 2 <=> second 60 degree sector etc.

this means that the moment we transition to state (POS1=1, POS2=0 POS3=0) we are at 0 electrical degrees
*/

// sector_lookup_table[POS3][POS2][POS1];
int sector_lookup_table[2][2][2];
//values set in init() function


float pos_sens_deltas [POS_SENS_DELTAS_SIZE];

int pos_sens_last_SysTick_count;


void pos_sens_init (void);

//	returns time spent in previous sectors								- input address of existing variable array
// which sector we are at currently (see above for position convention) - input address of existing variable
// how much time has elapsed since we entered this sector				- input address of existing variable
void get_Data_Pos (float * previous_deltas, int * current_sector, float * time_in_current_sector);


#endif /* POSITIONSENSORS_H_ */