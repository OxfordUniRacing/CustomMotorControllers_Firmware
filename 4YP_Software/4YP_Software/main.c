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
	
	
	//Initialise dead time on PWMs 
	pwm_deadtime_init();	
	
	
	pwm_enable_all();
	adc_enable_all();
	gpio_set_pin_level(PIN_GPIO_DCDC_ON_OFF, true);
	
	timer_start(&ENCODER_A);
	timer_start(&ENCODER_B);
	
	
	//test timers for encoder
	Timer_Counter_Initial_Test();
	
	
	//Runs first slow spin test of motor slowly increasing the angle so the rotor can catch up
	//first_slow_spin();

	/* Replace with your application code */
	while (1) {
		
		
		Current_Voltage_Inital_Test();
		delay_ms(500);
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
