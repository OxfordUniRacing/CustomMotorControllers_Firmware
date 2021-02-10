/*
 * GetCurrent.c
 *
 * Created: 13/01/2021 17:36:13
 *  Author: Ben
 */ 

#include "GetCurrent.h"



float ccfr(uint16_t r){
		
	float Ix;		//Individual current
		
	//Calculates the current from the ADC Read
	Ix = (5-r)/0.005;
	return Ix;
}

void GetCurrents(float* Ip){
	//Update stored values for Current A, B and C

	Ip[0] = ccfr(adc_read(ADC_CURRENT_A));
	Ip[1] = ccfr(adc_read(ADC_CURRENT_B));
	Ip[2] = ccfr(adc_read(ADC_CURRENT_C));

}
