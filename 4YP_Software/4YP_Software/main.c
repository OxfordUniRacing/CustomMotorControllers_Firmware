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

#include "hpl_dma.h"
//#include "hri_afec_e70b.h"


static uint32_t afec_buf[12];

static void dma_interrupt(struct _dma_resource *resource)
{
	/* period interrupt */
	printf("interrupt - %i %i %i %i %i %i %i %i %i %i %i %i  \n", (int)afec_buf[0],(int)afec_buf[1],(int)afec_buf[2],(int)afec_buf[3],(int)afec_buf[4],(int)afec_buf[5],(int)afec_buf[6],(int)afec_buf[7],(int)afec_buf[8],(int)afec_buf[9],(int)afec_buf[10],(int)afec_buf[11]);
}

int main(void)
{
	
	
	
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	//CAN is currently disabled
	//Temp 3 currently disabled as pin is shared with edbg com
	
		
	gpio_set_pin_level(PIN_USER_LED, true);
	//_dma_set_source_address(0,(void *) ((Afec *)((&ADC_0)->device.hw) + AFEC_LCDR_OFFSET));
	_dma_set_source_address(0,(void *)0x4003C020);
	_dma_set_destination_address(0,afec_buf);
	_dma_set_data_amount(0,4*6);
	
	struct _dma_resource **res;
	_dma_get_channel_resource(res, 0);
	(*res)->dma_cb.transfer_done = dma_interrupt;
	
	//NVIC_DisableIRQ(XDMAC_IRQn);
	_dma_set_irq_state(0,DMA_TRANSFER_COMPLETE_CB,true);
	//_dma_set_irq_state(0,DMA_TRANSFER_ERROR_CB,true);
	//NVIC_EnableIRQ(XDMAC_IRQn);
	
	_dma_enable_transaction(0,true);
	
	pwm_enable_all();
	adc_enable_all();
	gpio_set_pin_level(PIN_GPIO_DCDC_ON_OFF, true);
	
	timer_start(&ENCODER_A);
	timer_start(&ENCODER_B);
	
	
	//test timers for encoder
	//Timer_Counter_Initial_Test();
	
	
	//Runs first slow spin test of motor slowly increasing the angle so the rotor can catch up
	//first_slow_spin();

	/* Replace with your application code */
	while (1) {
		
		gpio_set_pin_level(PIN_USER_LED,true);
		delay_ms(500);
		
		int a = (int) adc_read(ADC_TEMP_2);
		printf("main - %i \n",a);
		printf("main - %i %i %i %i %i %i %i %i %i %i %i %i  \n", (int)afec_buf[0],(int)afec_buf[1],(int)afec_buf[2],(int)afec_buf[3],(int)afec_buf[4],(int)afec_buf[5],(int)afec_buf[6],(int)afec_buf[7],(int)afec_buf[8],(int)afec_buf[9],(int)afec_buf[10],(int)afec_buf[11]);
		//AFEC0_Handler
		_dma_set_destination_address(0,afec_buf);
		_dma_set_data_amount(0,4*6);
		_dma_enable_transaction(0,true);
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
