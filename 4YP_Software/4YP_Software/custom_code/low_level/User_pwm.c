/*
 * User_pwm.c
 *
 * Created: 11/01/2021 16:40:10
 *  Author: Ruzhev
 */ 


/*
The following modifications need to be done to hpl_pwm.c:

In _pwm_init function, add the following 3 lines to the corresponding place

	// Init Channel 
	for (i = 0; i < cfg->ch_num; i++) {
		ch = cfg->ch + i;
->		hri_pwm_write_CMR_reg(hw, ch->index, ch->mode | PWM_CMR_DTE);					//enable dead time
		//hri_pwmchnum_set_CMR_DTE_bit((void *) &((Pwm *)hw)->PwmChNum[ch->index]);		//another less elegant way of enabling dead time
->		hri_pwm_set_DT_DTH_bf(hw, ch->index, 10);										//set high side deadtime in PWM clock counts
->		hri_pwm_set_DT_DTL_bf(hw, ch->index, 10);										//set high side deadtime in PWM clock counts
		hri_pwm_write_CDTY_reg(hw, ch->index, ch->duty_cycle);
		hri_pwm_write_CPRD_reg(hw, ch->index, ch->period);
	}


Remove PWM Handler functions from _pwm_init

*/

#include "User_pwm.h"

#include "User_Config.h"
#include "driver_init.h"
#include <hpl_pwm_base.h>
#include <hpl_pwm.h>
#include <hpl_pwm_config.h>



void PWM0_Handler(void){
	
}



//enable/disable pwm pins
//also sets periods and other variables which might not have been set at initialization
void pwm_enable_all(void){
	//note it's possible to individual channels, but not through these functions
	//functions from hal_pwm.h
	
	
	//enable hardware interrupt on PWM0 channel 0
	// we dont need redundant calls
	hri_pwm_set_IMR1_CHID0_bit(PWM0);
	
	//enable interrupt on PWM 0 and disable on PWM 1
	NVIC_DisableIRQ(PWM0_IRQn);
	NVIC_DisableIRQ(PWM1_IRQn);
	
	
	
	//enable PWM0 and PWM1
	pwm_enable(&PWM_0);
	pwm_enable(&PWM_1);
		
	//set period and initial duty cycle
	//initial duty cycle = 0.5 * period <=> no current output for an H-bridge type driver
	pwm_set_parameters(&PWM_0, PWM_PERIOD, PWM_PERIOD>>1);
	pwm_set_parameters(&PWM_1, PWM_PERIOD, PWM_PERIOD>>1);
	
	
	
}


void pwm_disable_all(void){
	//functions from hal_pwm.h
	
	//disable PWM0 and PWM1
	pwm_disable(&PWM_0);
	pwm_disable(&PWM_1);
}

//sets individual channel pwm duty cycle
void pwm_set_duty(struct  pwm_descriptor * const descr, const uint8_t channel, const pwm_period_t duty_cycle){
	// based on available code from <hpl_pwm.h>
	
	hri_pwm_write_CDTYUPD_reg(descr->device.hw, channel, duty_cycle);
	//hri_pwm_write_CPRDUPD_reg(device->hw, cfg->ch[i].index, period);			//period is constant; only altering duty cycle
}
