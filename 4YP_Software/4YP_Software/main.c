#include <atmel_start.h>


#include "User_Config.h"
#include "User_pwm.h"
#include "User_adc.h"
#include "Initial_Testing.h"
#include "Encoder.h"
#include "PositionSensors.h"
#include "AnalogSensorConversion.h"
#include "Control.h"
#include "ControlStartup.h"


//add these 3 lines before every instance of arm_math.h ;
//configuration macro to enable the correct code within the library
#ifndef ARM_MATH_CM7
#define ARM_MATH_CM7
#endif
//always have atmel_start.h before arm_math.h -> seems to be fixing some issiues
//#include <atmel_start.h>	//already inclued earlier in the file
#include "arm_math.h"



//for enabling the FPU
/** Address for ARM CPACR */
#define ADDR_CPACR 0xE000ED88
/** CPACR Register */
#define REG_CPACR (*((volatile uint32_t *)ADDR_CPACR))
/**
* \brief Enable FPU
*/
__always_inline static void fpu_enable(void)
{
	//the commented out sections were causing errors. Hope that this doesnt cause the program to explode
	//irqflags_t flags;
	//flags = cpu_irq_save();
	REG_CPACR |= (0xFu << 20);
	__DSB();
	__ISB();
	//cpu_irq_restore(flags);
}
__STATIC_INLINE void SCB_EnableDCache_custom (void)
{
	#if defined (__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
	uint32_t ccsidr;
	uint32_t sets;
	uint32_t ways;

	SCB->CSSELR = 0U; /*(0U << 1U) | 0U;*/  /* Level 1 data cache */
	__DSB();

	ccsidr = SCB->CCSIDR;

	/* invalidate D-Cache */
	sets = (uint32_t)(CCSIDR_SETS(ccsidr));
	do {
		ways = (uint32_t)(CCSIDR_WAYS(ccsidr));
		do {
			SCB->DCISW = (((sets << SCB_DCISW_SET_Pos) & SCB_DCISW_SET_Msk) |
			((ways << SCB_DCISW_WAY_Pos) & SCB_DCISW_WAY_Msk)  );
			#if defined ( __CC_ARM )
			__schedule_barrier();
			#endif
		} while (ways-- != 0U);
	} while(sets-- != 0U);
	__DSB();

	SCB->CCR |=  (uint32_t)SCB_CCR_DC_Msk;  /* enable D-Cache */

	__DSB();
	__ISB();
	#endif
}

/*
if(SCB->CCR & (uint32_t)SCB_CCR_DC_Msk)
{
	// D-cache already enabled, return
	// (reenabling it will halt the system)
	return;
}
*/
int main(void)
{
	
	
	
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	//CAN is currently disabled
	//Temp 3 currently disabled as pin is shared with edbg com
	
	//enable performance optimising features
	fpu_enable();
	if(SCB->CCR & (uint32_t)SCB_CCR_DC_Msk){
		// D-cache already enabled, return
		// (reenabling it will halt the system)
	}else{
		SCB_EnableDCache();
	}
	SCB_EnableICache();
	
	
	
	/* Additional User initialisation */
	dma_adc_init();
	pwm_init_user();
	encoder_init();
	pos_sens_init();
	
	/* Enable all devices */
	pwm_enable_all();
	adc_enable_all();
	

	calibrate_curr_sensors(10);	//both PWM and ADC need to be enabled to calibrate the current sensors
	//enable the dma to get the offset. It will re enable itself for the other data points
	dma_adc_0_enable_for_one_transaction();
	//dma_adc_1_enable_for_one_transaction();
	delay_ms(100);		//some time to get the data
	curr_A_offset = curr_A_offset/10;
	curr_B_offset = curr_B_offset/10;
	
	dma_adc_1_enable_for_one_transaction();
	delay_ms(1);		//some time to get the data
	dma_adc_1_enable_for_one_transaction();
	delay_ms(1);		//some time to get the data
	dma_adc_1_enable_for_one_transaction();
	delay_ms(1);		//some time to get the data
	dma_adc_1_enable_for_one_transaction();
	delay_ms(1);		//some time to get the data
	dma_adc_1_enable_for_one_transaction();
	delay_ms(1);		//some time to get the data
	dma_adc_1_enable_for_one_transaction();
	delay_ms(1);		//some time to get the data
	dma_adc_1_enable_for_one_transaction();
	delay_ms(1);		//some time to get the data
	dma_adc_1_enable_for_one_transaction();
	delay_ms(1);		//some time to get the data
	dma_adc_1_enable_for_one_transaction();
	delay_ms(1);		//some time to get the data
	dma_adc_1_enable_for_one_transaction();
	delay_ms(1);		//some time to get the data
	
	
	curr_C_offset = curr_C_offset/10;
	printf("offset A %f \t B %f \t C %f \n",curr_A_offset,curr_B_offset,curr_C_offset);
	gpio_set_pin_level(PIN_GPIO_DCDC_ON_OFF, true);		//enables the DC-DC converter for the HV side
	
	timer_start(&ENCODER_A);
	timer_start(&ENCODER_B);
	
	dma_adc_0_enable_continuously();
	dma_adc_1_enable_continuously();
	
	delay_ms(100);
	printf("asdf\n");
	
	Init_Control();
	init_LPF();
	//enable_control();
	//----------------------------------------End of Startup Code--------------------------------------------------
	
	//Current_Offset_And_Timing_Test();
	
	delay_ms(500);
	printf("Initiated \n");
	//first_slow_spin();
	//while(1){}
		
	printf("Starting D axis alignment \n");
	
	pwm_set_duty(PWM_PHASE_A, 500);
	pwm_set_duty(PWM_PHASE_B, (PWM_PERIOD-1));
	pwm_set_duty(PWM_PHASE_C, (PWM_PERIOD-1));
	delay_ms(2000);

	
	
	
	
	encoder_record_Daxis_offset();
	printf("Finished D axis alignment \n");
	/*
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
	*/
	enable_control();
	while(1){}
	
	//test timers for encoder
	//Timer_Counter_Initial_Test();
	//test encoder itself
	//Encoder_Initial_Test();
	
	//test position sensor
	//POS_Sensor_Initial_Test();
	

	//for seeing the square waves on just one channel
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
