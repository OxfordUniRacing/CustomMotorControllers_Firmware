/*
 * GetCurrent.h
 *
 * Created: 13/01/2021 17:35:50
 *  Author: Ben
 */ 


#ifndef GETCURRENT_H_
#define GETCURRENT_H_

#include "User_adc.h"
#include "User_Config.h"


float ccfr(uint16_t r);

void GetCurrents(float* Ip);


#endif /* GETCURRENT_H_ */