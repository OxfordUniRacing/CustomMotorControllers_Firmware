/*
 * AnalogSensorConversion.c
 *
 * Created: 28/02/2021 17:48:37
 *  Author: Ruzhev
 */ 


#include "AnalogSensorConversion.h"




//allow seperate current conversion functions so we can calibrate the sensors
float reconstruct_curr_A(uint32_t raw_current_data){
	float I = (5-raw_current_data)/0.005;
	return I;
}
float reconstruct_curr_B(uint32_t raw_current_data){
	float I = (5-raw_current_data)/0.005;
	return I;
}
float reconstruct_curr_C(uint32_t raw_current_data){		
	float I = (5-raw_current_data)/0.005;
	return I;
}



float reconstruct_bus_voltage(uint32_t raw_voltage_data){
	
	return 0;
}


float reconstruct_temp(uint32_t raw_temp_data){
	
	return 0;
}
float reconstruct_motor_temp(uint32_t raw_temp_data){
	
	return 0;
}