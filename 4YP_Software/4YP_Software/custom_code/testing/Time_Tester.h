/*
 * Time_Tester.h
 *
 * Created: 24/04/2021 13:09:58
 *  Author: Ruzhev
 */ 


#ifndef TIME_TESTER_H_
#define TIME_TESTER_H_

//atmel start gives access to the SysTick
#include <atmel_start.h>


int timestamp;

void time_record_timestamp(void);
float time_get_delta_us(void);


#endif /* TIME_TESTER_H_ */