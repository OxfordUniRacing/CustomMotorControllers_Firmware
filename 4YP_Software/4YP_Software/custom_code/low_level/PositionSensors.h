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

#define POS_SENS_DELTAS_SIZE 8
float pos_sens_deltas [POS_SENS_DELTAS_SIZE];

int pos_sens_last_SysTick_count;


void pos_sens_init (void);

//returns how many time steps are recorded in the times array
int pos_sens_get_deltas_us (float * deltas);


#endif /* POSITIONSENSORS_H_ */