/*
 * Encoder.h
 *
 * Created: 23/02/2021 15:51:10
 *  Author: Ruzhev
 */ 


/*

This file contains all functions relating to working with the encoder

The .c file also contains an explanation of what needs to be modified in the system files to configure the Timer counters


*/


#ifndef ENCODER_H_
#define ENCODER_H_

#include "User_Config.h"


int encoder_rotations;	//for total rotations on Z
int encoder_inital_offset;	//to store the offet between the last reset point (start point) and the place where Z counts;


void encoder_enable(void);
float encoder_get_angle(void);
int encoder_get_counter(void);
int encoder_get_rotations(void);




#endif /* ENCODER_H_ */