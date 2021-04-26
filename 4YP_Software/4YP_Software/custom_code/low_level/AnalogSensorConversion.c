/*
 * AnalogSensorConversion.c
 *
 * Created: 28/02/2021 17:48:37
 *  Author: Ruzhev
 */ 


#include "AnalogSensorConversion.h"
#include "User_Config.h"

#include <atmel_start.h>



void calibrate_curr_sensors(int number_of_averages){
	//measures the current sensor offset at no current
	


	//MAKE SURE THE PWMS ARE ENABLED TO DO THE ADC CONVERSION
	
	
	curr_A_offset = 0;
	curr_B_offset = 0;
	curr_C_offset = 0;
	
	
	//tell the adcs to record the data needed
	calibrate_curr_sensors_counter_0 = number_of_averages;
	calibrate_curr_sensors_counter_1 = number_of_averages;
	
	
	


}

//allow seperate current conversion functions so we can calibrate the sensors
float reconstruct_curr_A(uint32_t raw_current_data){
	float I = (raw_data_to_voltage(raw_current_data)   -   curr_A_offset)/CURR_A_SLOPE;
	return I;
}
float reconstruct_curr_B(uint32_t raw_current_data){
	float I = (raw_data_to_voltage(raw_current_data)   -   curr_B_offset)/CURR_B_SLOPE;
	return I;
}
float reconstruct_curr_C(uint32_t raw_current_data){		
	float I = (raw_data_to_voltage(raw_current_data)   -   curr_C_offset)/CURR_C_SLOPE;
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


float raw_data_to_voltage(uint32_t analog_data){
	//12 bit ADCs
	//Vref = 3.3V
	float voltage = (((float) analog_data) / (4096)) * 3.3;
	//printf("raw data - %i \t voltage - %f \n",(int) analog_data, voltage);
	return voltage;
}