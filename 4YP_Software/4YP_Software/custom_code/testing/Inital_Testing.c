/*
 * Inital_Testing.c
 *
 * Created: 10/02/2021 17:34:35
 *  Author: Ruzhev
 */ 
/*
	general file with simple routines for testing the basic functionality of the MCU
*/

#include "Initial_Testing.h"

#include "User_Config.h"
#include "User_pwm.h"
#include "User_adc.h"
#include "GetCurrent.h"

#include <atmel_start.h>


void Temp_Initial_Test (void){
	//get ADC Temp reading and display values
	
	int temp_test_data [7];
	
	temp_test_data [0] = adc_read(ADC_TEMP_1);
	temp_test_data [1] = adc_read(ADC_TEMP_2);
	//Temp 3 currently disabled;
	//temp_data [2] = adc_read(ADC_TEMP_3);
	temp_test_data [3] = adc_read(ADC_TEMP_4);
	temp_test_data [4] = adc_read(ADC_TEMP_5);
	temp_test_data [5] = adc_read(ADC_TEMP_6);
	
	temp_test_data [6] = adc_read(ADC_TEMP_MOTOR);
	
	printf("\n ADC Temperature Testing -----------------\n");
	printf("Raw Data \n");
	printf("Temp1 = %i \t\t Temp2 = %i \t\t Temp3 = %i \n",temp_test_data[0], temp_test_data[1], temp_test_data[2]);
	printf("Temp4 = %i \t\t Temp5 = %i \t\t Temp6 = %i \n",temp_test_data[3], temp_test_data[4], temp_test_data[5]);
	printf("Temp Motor = %i  \n",temp_test_data[6]);
	
	
}

void Current_Voltage_Inital_Test (void){
	//Get Current Sensor and High Voltage reading and display values
	
	int current_test_data [3];
	
	current_test_data [0] = adc_read(ADC_CURRENT_A);
	current_test_data [1] = adc_read(ADC_CURRENT_B);
	current_test_data [2] = adc_read(ADC_CURRENT_C);
	
	int voltage_test_data;
	
	voltage_test_data = adc_read(ADC_SUPPL_VOLTAGE);
	
	printf("\n Current and Voltage Testing -----------------\n");
	printf("Raw Data \n");
	printf("CurrA = %i \t\t CurrB = %i \t\t CurrC = %i \n",current_test_data[0], current_test_data[1], current_test_data[2]);
	printf("Supply Voltage = %i  \n",voltage_test_data);
}


void PWM_Initial_Test (void);
void POS_Sensor_Initial_Test (void);