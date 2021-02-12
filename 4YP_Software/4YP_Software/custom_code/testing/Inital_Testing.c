/*
 * Inital_Testing.c
 *
 * Created: 10/02/2021 17:34:35
 *  Author: Ruzhev
 */ 
/*
	general file with simple routines for testing the basic functionality of the MCU
*/

//add these 3 lines before every instance of arm_math.h ;
//configuration macro to enable the correct code within the library
#ifndef ARM_MATH_CM7
#define ARM_MATH_CM7
#endif
//always have atmel_start.h before arm_math.h -> seems to be fixing some issiues
#include <atmel_start.h>
#include "arm_math.h"

#include "User_Config.h"
#include "User_pwm.h"
#include "User_adc.h"
#include "GetCurrent.h"



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

void first_slow_spin (void){
	float omega = 94.25;    //Gives electrical frequency of 15Hz (one spin of the rotor every second)
	float T = 0;
	float V_supply = 20;
	float V_pp_test = 5;
	while (1)
	{
		float pwm_testing_a, pwm_testing_b, pwm_testing_c;
		pwm_testing_a = (sin(omega*T)+1)*500*V_pp_test/V_supply;
		pwm_testing_b = (sin(omega*T+PI/3)+1)*500*V_pp_test/V_supply;
		pwm_testing_c = (sin(omega*T-PI/3)+1)*500*V_pp_test/V_supply;
		
		pwm_set_duty(PWM_PHASE_A,pwm_testing_a);
		pwm_set_duty(PWM_PHASE_B,pwm_testing_b);
		pwm_set_duty(PWM_PHASE_C,pwm_testing_c);
		
		delay_us(333)
		T = T + 0.000333;			//Updates the duty cycle every switch at 3kHz switching frequency
	}
}


void PWM_Initial_Test (void);
void POS_Sensor_Initial_Test (void);