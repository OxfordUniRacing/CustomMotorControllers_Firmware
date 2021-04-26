/*
 * AnalogSensorConversion.h
 *
 * Created: 28/02/2021 17:47:22
 *  Author: Ruzhev
 */ 


#ifndef ANALOGSENSORCONVERSION_H_
#define ANALOGSENSORCONVERSION_H_

//this file contains functions to turn the raw ADC readings to useful data (currents, voltages, temperatures)
#include <stdint.h>

float curr_A_offset, curr_B_offset, curr_C_offset;	// measured in volts

int calibrate_curr_sensors_counter_0;
int calibrate_curr_sensors_counter_1;
void calibrate_curr_sensors(int number_of_averages);

//allow seperate current conversion functions so we can calibrate the sensors
float reconstruct_curr_A(uint32_t raw_current_data);
float reconstruct_curr_B(uint32_t raw_current_data);
float reconstruct_curr_C(uint32_t raw_current_data);

float reconstruct_bus_voltage(uint32_t raw_voltage_data);

float reconstruct_temp(uint32_t raw_temp_data);
float reconstruct_motor_temp(uint32_t raw_temp_data);

float raw_data_to_voltage(uint32_t analog_data);




#endif /* ANALOGSENSORCONVERSION_H_ */