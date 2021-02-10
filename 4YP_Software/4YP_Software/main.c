#include <atmel_start.h>


#include "User_Config.h"
#include "User_pwm.h"
#include "User_adc.h"
#include "GetCurrent.h"


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
	
	pwm_enable_all();
	adc_enable_all();
	gpio_set_pin_level(PIN_GPIO_DCDC_ON_OFF, true);

	/* Replace with your application code */
	while (1) {
		
		gpio_set_pin_level(PIN_USER_LED, false);
		//pwm_set_duty(PWM_PHASE_A, 8000);
		delay_ms(2000);
		printf("this is working\n");
		
		gpio_set_pin_level(PIN_USER_LED, true);
		//pwm_set_duty(PWM_PHASE_A, 3000);
		delay_ms(2000);
	}
}
