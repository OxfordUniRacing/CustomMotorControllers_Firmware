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

		
		

		hri_tc_write_CMR_reg(hw, 0, TC_CMR_TCCLKS_XC2 | TC_CMR_WAVE_Msk);						//wave mode (counting up); external clock
		hri_tc_write_RA_reg(hw, 0, ra);
		hri_tc_write_RB_reg(hw, 0, rb);
		hri_tc_write_EMR_reg(hw, 0, ext_mode);
		hri_tc_write_RC_reg(hw, 0, rc);
		hri_tc_set_IMR_reg(hw, 0, 0);	//no interrupts
		
		hri_tc_write_CMR_reg(hw, 1, TC_CMR_TCCLKS_XC2 | TC_CMR_WAVE_Msk | TC_CMR_CLKI_Msk);		//wave mode (counting up); external clock; inverted
		hri_tc_write_RA_reg(hw, 1, ra);
		hri_tc_write_RB_reg(hw, 1, rb);
		hri_tc_write_EMR_reg(hw, 1, ext_mode);
		hri_tc_write_RC_reg(hw, 1, rc);
		hri_tc_set_IMR_reg(hw, 1, 0);	//no interrupts
		
		hri_tc_write_FMR_reg(hw, cfg->fmr);
		

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




static void Encoder_Z_Interrupt (void){
	//Interrupt function for Z triggers
	//Checks count on A and B in case something is wrong
	
	//get counter value
	int encoder_counter_no_offset = encoder_get_counter();
	
	
	//if first rotation then record the offset
	if(encoder_rotations == 0){
		encoder_inital_offset = encoder_counter_no_offset;
	}else{
		//if not first rotation, check if the number of pulses this rotation was within margin of counting error
		
		//we know that encoder steps is 2^x
		// by doing the bitwise operation we effectively subtract the number of rotations* encoder steps
		// and we also eliminate any problems with overflowing counters
		unsigned int delta = encoder_counter_no_offset & (ENCODER_STEPS - 1);
		
		//ideally delta should be zero
		// tolerance up to +2 (0<=delta <=2) or -2 (ENCODER_STEPS-3<=delta)
		if( (delta <=2) || (delta >= ENCODER_STEPS-3) ){
			//all is good
		}else{
			//something is wrong
		}
		
	}
	
	
	//increment rotation counter
	encoder_rotations ++;
}

void encoder_enable(void){
	//start Timer counters
	//set variables to zero
	//attach Z interrupt function
	
	timer_start(&ENCODER_A);
	timer_start(&ENCODER_B);
	
	encoder_rotations = 0;
	encoder_inital_offset = 0;
	
	ext_irq_register(PIO_PA26_IDX,Encoder_Z_Interrupt);
}

float encoder_get_angle(void){
	int encoder_counter_no_offset = encoder_get_counter();
	
	//we know that encoder steps is 2^x
	// by doing the bitwise operation we effectively subtract the number of rotations* encoder steps
	// and we also eliminate any problems with overflowing counters
	int current_counter = encoder_counter_no_offset & (ENCODER_STEPS - 1);
	
	//convert to radians and scale
	float angle = 2 * PI * ((float)(current_counter)) / ENCODER_STEPS;
	return angle;
}

int encoder_get_counter(void){
	//returns the counter compensatinfor the starting offset
	//if starting offset is 0 (which is the case before it was recorded) then it returns the actual counter
	
	
	//get A (first line) and B (second line) counters
	//note that rising and falling edges could be the other way round. It doesn't matter for the current implementation
	int encoder_counter_no_offset =	  ( int) hri_tc_read_CV_CV_bf(TC0,0) + ( int) hri_tc_read_CV_CV_bf(TC0,1) \
									+ ( int) hri_tc_read_CV_CV_bf(TC3,0) + ( int) hri_tc_read_CV_CV_bf(TC3,1) \
									- encoder_inital_offset;														//get the number of steps normalised to the starting offset
								
	return encoder_counter_no_offset;
}

int encoder_get_rotations(void){
	return encoder_rotations;
}
