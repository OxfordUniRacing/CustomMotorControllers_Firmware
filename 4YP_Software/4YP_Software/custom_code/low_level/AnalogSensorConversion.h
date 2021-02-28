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


//allow seperate current conversion functions so we can calibrate the sensors
float reconstruct_curr_A(uint32_t raw_current_data);
float reconstruct_curr_B(uint32_t raw_current_data);
float reconstruct_curr_C(uint32_t raw_current_data);

float reconstruct_bus_voltage(uint32_t raw_voltage_data);

float reconstruct_temp(uint32_t raw_temp_data);
float reconstruct_motor_temp(uint32_t raw_temp_data);




#endif /* ANALOGSENSORCONVERSION_H_ */