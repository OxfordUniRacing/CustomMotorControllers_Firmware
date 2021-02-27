#include <atmel_start.h>


#include "User_Config.h"
#include "User_pwm.h"
#include "User_adc.h"
#include "GetCurrent.h"
#include "Initial_Testing.h"


//add these 3 lines before every instance of arm_math.h ;
//configuration macro to enable the correct code within the library
#ifndef ARM_MATH_CM7
#define ARM_MATH_CM7
#endif
//always have atmel_start.h before arm_math.h -> seems to be fixing some issiues
//#include <atmel_start.h>	//already inclued earlier in the file
#include "arm_math.h"




int main(void)
{
	
	
	
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	//CAN is currently disabled
	//Temp 3 currently disabled as pin is shared with edbg com
	
	/* Additional User initialisation */
	dma_adc_init();
	pwm_init_user();
	encoder_init();
	pos_sens_init();
	
	/* Enable all devices */
	pwm_enable_all();
	adc_enable_all();
	gpio_set_pin_level(PIN_GPIO_DCDC_ON_OFF, true);		//enables the DC-DC converter for the HV side
	
	timer_start(&ENCODER_A);
	timer_start(&ENCODER_B);
	
	
	//----------------------------------------End of Startup Code--------------------------------------------------
	
	
	
	//test timers for encoder
	//Timer_Counter_Initial_Test();
	
	
	//Runs first slow spin test of motor slowly increasing the angle so the rotor can catch up
	//first_slow_spin();

	/* Replace with your application code */
	while (1) {
		
		gpio_set_pin_level(PIN_GPIO_6,true);
		gpio_set_pin_level(PIN_USER_LED,true);
		delay_ms(500);
		
		dma_adc_0_enable_for_one_transaction();
		dma_adc_1_enable_for_one_transaction();
		adc_async_start_conversion(&ADC_0);
		adc_async_start_conversion(&ADC_1);
		//printf("main - %i %i %i %i %i %i %i %i %i %i %i %i  \n", (int)afec_buf[0],(int)afec_buf[1],(int)afec_buf[2],(int)afec_buf[3],(int)afec_buf[4],(int)afec_buf[5],(int)afec_buf[6],(int)afec_buf[7],(int)afec_buf[8],(int)afec_buf[9],(int)afec_buf[10],(int)afec_buf[11]);
		//AFEC0_Handler
		
		gpio_set_pin_level(PIN_GPIO_6,false);
		gpio_set_pin_level(PIN_USER_LED,false);
		delay_ms(500);
		
		
		//Current_Voltage_Inital_Test();
		//delay_ms(500);
		/*
		gpio_set_pin_level(PIN_USER_LED, false);
		pwm_set_duty(PWM_PHASE_A, 800);
		delay_ms(2000);
		printf("this is \tworking\n");
		
		gpio_set_pin_level(PIN_USER_LED, true);
		pwm_set_duty(PWM_PHASE_A, 300);
		delay_ms(2000);*/
	}
}
