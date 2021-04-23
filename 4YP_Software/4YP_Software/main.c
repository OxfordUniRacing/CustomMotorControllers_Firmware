#include <atmel_start.h>


#include "User_Config.h"
#include "User_pwm.h"
#include "User_adc.h"
#include "Initial_Testing.h"
#include "Encoder.h"
#include "PositionSensors.h"
#include "AnalogSensorConversion.h"
#include "Control.h"


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
	calibrate_curr_sensors();	//both PWM and ADC need to be enabled to calibrate the current sensors
	gpio_set_pin_level(PIN_GPIO_DCDC_ON_OFF, true);		//enables the DC-DC converter for the HV side
	
	timer_start(&ENCODER_A);
	timer_start(&ENCODER_B);
	
	dma_adc_0_enable_continuously();
	dma_adc_1_enable_continuously();
	//enable_control();
	//----------------------------------------End of Startup Code--------------------------------------------------
	
	Current_Offset_Test();
	delay_ms(500);
	printf("Initiated \n");
	//first_slow_spin();
	
	printf("Starting D axis alignment \n");
	pwm_set_duty(PWM_PHASE_A, 0);
	pwm_set_duty(PWM_PHASE_B, (PWM_PERIOD-1));
	pwm_set_duty(PWM_PHASE_C, (PWM_PERIOD-1));
	
	

	delay_ms(5000);
	encoder_record_Daxis_offset();
	printf("Finished D axis alignment \n");

	Vd_aim = 0;
	Vq_aim = 1;
	enable_control();
	
	
	delay_ms(3000);
	printf("Increasing PWM \n");
	for(int i =0;i< 10;i++){
		Vq_aim +=0.1;
		delay_us(100);
	}
	printf("Vq aim - %f \n",Vq_aim);
	
	while(1){}
	
	//test timers for encoder
	//Timer_Counter_Initial_Test();
	//test encoder itself
	//Encoder_Initial_Test();
	
	//test position sensor
	//POS_Sensor_Initial_Test();
	

	//for seeing the suqre waves on just one channel
	//pwm_disable(&PWM_0);
	//pwm_set_duty(PWM_PHASE_C, 950);
	//while(1){}

	//Control_Function_Test();
	

	
	//test automatic current sensor offset voltage
	//Current_Offset_Test();
	
	
	//Runs first slow spin test of motor slowly increasing the angle so the rotor can catch up
	//first_slow_spin();

	/* Replace with your application code */
	while (1) {
	}
}
