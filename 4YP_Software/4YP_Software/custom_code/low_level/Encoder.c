/*
 * Encoder.c
 *
 * Created: 23/02/2021 15:51:35
 *  Author: Ruzhev
 */ 

/*
Alteration to system files

In order to configure the Timer Counters we need to modify the hpl_tc.c file
ATMEL Start doesn't have great support for dealing with Timers
Namely by default only channel 0 of each timer is initialized, but we need both channels 0 and 1
need to configure the timers to Wave mode with external clock
Finally remove interrupts as they are not needed

Changes that need to be made:

In _timer_init():
Replace function with 
	
		//	Modified function to enable not only channel 0, but also channel 1 of the timer counters
		//	Clock is set to external 2 (TCLK 2 and TCLK 11 for Encoder A and B respectively)
		
		
		struct tc_configuration *cfg     = get_cfg(hw);
		
		uint32_t ra = cfg->ra;
		uint32_t rb = cfg->rb;
		uint32_t rc = cfg->rc;
		uint32_t ext_mode = cfg->ext_mode;
		
		

		device->hw = hw;
		ASSERT(ARRAY_SIZE(_tcs));

		
		

		hri_tc_write_CMR_reg	(hw, 0, TC_CMR_TCCLKS_XC2 | TC_CMR_WAVE_Msk);						//wave mode (counting up); external clock
		hri_tc_write_RA_reg		(hw, 0, ra);
		hri_tc_write_RB_reg		(hw, 0, rb);
		hri_tc_write_EMR_reg	(hw, 0, ext_mode);
		hri_tc_write_RC_reg		(hw, 0, rc);
		hri_tc_set_IMR_reg		(hw, 0, 0);	//no interrupts
		
		hri_tc_write_CMR_reg	(hw, 1, TC_CMR_TCCLKS_XC2 | TC_CMR_WAVE_Msk | TC_CMR_CLKI_Msk);		//wave mode (counting up); external clock; inverted
		hri_tc_write_RA_reg		(hw, 1, ra);
		hri_tc_write_RB_reg		(hw, 1, rb);
		hri_tc_write_EMR_reg	(hw, 1, ext_mode);
		hri_tc_write_RC_reg		(hw, 1, rc);
		hri_tc_set_IMR_reg		(hw, 1, 0);	//no interrupts
		
		hri_tc_write_FMR_reg	(hw, cfg->fmr);
		

		_tc_init_irq_param(hw, device);
		NVIC_DisableIRQ(cfg->irq);
		NVIC_ClearPendingIRQ(cfg->irq);
		NVIC_EnableIRQ(cfg->irq);

		return ERR_NONE;
		
		
		
In all other functions add a copy of the existing funtion for channel 1 as well like so:
hri_tc_write_RC_reg(device->hw, 0, clock_cycles);	//original
hri_tc_write_RC_reg(device->hw, 1, clock_cycles);	//newly added
*/





#include "Encoder.h"

#include "hri_tc_e70b.h"

//add these 3 lines before every instance of arm_math.h ;
//configuration macro to enable the correct code within the library
#ifndef ARM_MATH_CM7
#define ARM_MATH_CM7
#endif
//always have atmel_start.h before arm_math.h -> seems to be fixing some issiues
#include <atmel_start.h>
#include "arm_math.h"

#include "hpl_pmc.h"




static void Encoder_Z_Interrupt (void){
	//Interrupt function for Z triggers
	//Checks count on A and B in case something is wrong
	
	//get counter value
	int encoder_counter_no_offset = encoder_get_counter();
	
	//printf("Z interrupt - %i \t; offset = %i \n", encoder_counter_no_offset, encoder_inital_offset);
	
	
	//if first rotation then record the offset
	if(encoder_num_Z_interrupts == 0){
		encoder_Z_offset = encoder_counter_no_offset;
	}else{
		//if not first rotation, check if the number of pulses this rotation was within margin of counting error
		
		//we know that encoder steps is 2^x
		// by doing the bitwise operation we effectively subtract the number of rotations* encoder steps
		// and we also eliminate any problems with overflowing counters
		unsigned int delta = (encoder_counter_no_offset - encoder_Z_offset) & (ENCODER_STEPS - 1);
		
		printf("Z interrupt - delta = %i \n", delta);
		
		//ideally delta should be zero
		// tolerance up to +2 (0<=delta <=2) or -2 (ENCODER_STEPS-3<=delta)
		if( (delta <= ENCODER_MAX_DELTA) || (delta >= ENCODER_STEPS-1 - ENCODER_MAX_DELTA) ){
			//all is good
			}else{
			//something is wrong
			//printf("error = %i\n",(int) delta);
			//zero out the initial offset coutner to not corrupt future data
			//do this to both Z offset and D axis offset
			encoder_Z_offset += delta;
			encoder_Daxis_offset += delta;
		}
		
	}
	
	
	//increment rotation counter
	encoder_num_Z_interrupts ++;
}

void encoder_init(void){
	//default driver_init.c only enables the clock on channel 0
	//make sure all clocks are enabled
	_pmc_enable_periph_clock(ID_TC0_CHANNEL0);
	_pmc_enable_periph_clock(ID_TC0_CHANNEL1);
	
	_pmc_enable_periph_clock(ID_TC3_CHANNEL0);
	_pmc_enable_periph_clock(ID_TC3_CHANNEL1);
	
	
	//enable external interrupt on the Z line 
	ext_irq_register(PIO_PB13_IDX,Encoder_Z_Interrupt);
	
	NVIC_EnableIRQ	(PIOB_IRQn);
	NVIC_SetPriority(PIOB_IRQn, IRQ_PRIORITY_ENCODER_Z);
	
	//make sure interrupts from the Timer counters are disabled
	NVIC_DisableIRQ			(TC0_IRQn);
	NVIC_ClearPendingIRQ	(TC0_IRQn);
	NVIC_DisableIRQ			(TC1_IRQn);
	NVIC_ClearPendingIRQ	(TC1_IRQn);
	NVIC_DisableIRQ			(TC2_IRQn);
	NVIC_ClearPendingIRQ	(TC2_IRQn);
	NVIC_DisableIRQ			(TC3_IRQn);
	NVIC_ClearPendingIRQ	(TC3_IRQn);
	NVIC_DisableIRQ			(TC4_IRQn);
	NVIC_ClearPendingIRQ	(TC4_IRQn);
	NVIC_DisableIRQ			(TC5_IRQn);
	NVIC_ClearPendingIRQ	(TC5_IRQn);
	NVIC_DisableIRQ			(TC6_IRQn);
	NVIC_ClearPendingIRQ	(TC6_IRQn);
	NVIC_DisableIRQ			(TC7_IRQn);
	NVIC_ClearPendingIRQ	(TC7_IRQn);
	NVIC_DisableIRQ			(TC8_IRQn);
	NVIC_ClearPendingIRQ	(TC8_IRQn);
	NVIC_DisableIRQ			(TC9_IRQn);
	NVIC_ClearPendingIRQ	(TC9_IRQn);
	NVIC_DisableIRQ			(TC10_IRQn);
	NVIC_ClearPendingIRQ	(TC10_IRQn);
	NVIC_DisableIRQ			(TC11_IRQn);
	NVIC_ClearPendingIRQ	(TC11_IRQn);
}

void encoder_enable(void){
	//start Timer counters
	//set variables to zero
	//attach Z interrupt function
	
	timer_start(&ENCODER_A);
	timer_start(&ENCODER_B);
	
	encoder_num_Z_interrupts = 0;
	encoder_inital_offset = 0;
	
	encoder_Z_offset = 0;
	encoder_Daxis_offset = 0;
	encoder_error_offset = 0;
	
}

void encoder_get_angle(float * angl){
	//gets the angle with respect to the D axis
	//for some reason the function would return a random number if it was made to "float encoder_get_angle(void)". Therefore using a pointer to return the data
	int encoder_counter_no_offset = encoder_get_counter();
	
	//we know that encoder steps is 2^x
	// by doing the bitwise operation we effectively subtract the number of rotations* encoder steps
	// and we also eliminate any problems with overflowing counters
	int current_counter = (encoder_counter_no_offset - encoder_Daxis_offset) & (ENCODER_STEPS - 1);
	
	float current_counter_float = (float) current_counter;
	
	//convert to radians and scale
	*angl  = (2 * PI * current_counter_float / (ENCODER_STEPS));
	

}

int encoder_get_counter(void){
	//returns the counter compensatinfor the starting offset
	//if starting offset is 0 (which is the case before it was recorded) then it returns the actual counter
	
	
	//get A (first line) and B (second line) counters
	//note that rising and falling edges could be the other way round. It doesn't matter for the current implementation
	int encoder_counter_no_offset =	  ( int) hri_tc_read_CV_CV_bf(TC0,0) + ( int) hri_tc_read_CV_CV_bf(TC0,1) \
									+ ( int) hri_tc_read_CV_CV_bf(TC3,0) + ( int) hri_tc_read_CV_CV_bf(TC3,1);						
	
	return encoder_counter_no_offset;
}

int encoder_get_rotations(void){
	//MIGHT BE WRONG !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//the D axis implementation was introduced later and this function was not revised
	
	//if the counter < initial offset that means we have not completed the rotation yet (Z interrupt probably triggers somewhere in the middle of the rotation)
	return encoder_num_Z_interrupts - (encoder_get_counter () < encoder_Z_offset);
	
}



void encoder_record_Daxis_offset(void){
	//records the value at which we have a D axis
	encoder_Daxis_offset = encoder_get_counter();
}