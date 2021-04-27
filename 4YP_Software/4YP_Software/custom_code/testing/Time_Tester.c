/*
 * Time_Tester.c
 *
 * Created: 24/04/2021 13:10:11
 *  Author: Ruzhev
 */ 



#include "Time_Tester.h"







inline void time_record_timestamp(void){
	int current_systick = SysTick->VAL;
	timestamp = current_systick;
}

inline float time_get_delta_us(void){
	int current_systick = SysTick->VAL;
	int delta = timestamp - current_systick;
	if(delta <= 0) delta += (1<<24);
	//systick clock frequency equals MCU clock at 300MHz
	return (float) delta / 300;
}