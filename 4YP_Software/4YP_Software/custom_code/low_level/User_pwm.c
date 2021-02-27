/*
 * User_pwm.c
 *
 * Created: 11/01/2021 16:40:10
 *  Author: Ruzhev
 */ 



#include "User_pwm.h"

#include "User_Config.h"
#include "driver_init.h"
#include <hpl_pwm_base.h>
#include <hpl_pwm.h>
#include <hpl_pwm_config.h>



void pwm_0_callback(void){
	
}


void pwm_init_user(void){
	//initialises aditional funcitonality on the PWM channels
	
	
	//set dead time
	hri_pwm_set_CMR_reg		(PWM0, PWM_PHASE_A_CHANNEL, PWM_CMR_DTE);		//enable dead time on the corresponding channel
	hri_pwm_set_DT_DTH_bf	(PWM0, PWM_PHASE_A_CHANNEL, PWM_DEADTIME);		//set high side deadtime in PWM clock counts
	hri_pwm_set_DT_DTL_bf	(PWM0, PWM_PHASE_A_CHANNEL, PWM_DEADTIME);		//set low side deadtime

	hri_pwm_set_CMR_reg		(PWM0, PWM_PHASE_B_CHANNEL, PWM_CMR_DTE);		//enable dead time on the corresponding channel
	hri_pwm_set_DT_DTH_bf	(PWM0, PWM_PHASE_B_CHANNEL, PWM_DEADTIME);		//set high side deadtime in PWM clock counts
	hri_pwm_set_DT_DTL_bf	(PWM0, PWM_PHASE_B_CHANNEL, PWM_DEADTIME);		//set low side deadtime

	hri_pwm_set_CMR_reg		(PWM1, PWM_PHASE_C_CHANNEL, PWM_CMR_DTE);		//enable dead time on the corresponding channel
	hri_pwm_set_DT_DTH_bf	(PWM1, PWM_PHASE_C_CHANNEL, PWM_DEADTIME);		//set high side deadtime in PWM clock counts
	hri_pwm_set_DT_DTL_bf	(PWM1, PWM_PHASE_C_CHANNEL, PWM_DEADTIME);		//set low side deadtime
	
	
	//we want interrupt from one of the PWMs so that we can start the control loop
	//interrupt on PWM 0, channel 0 is enabled ; on PWM 1 is disabled
	hri_pwm_set_IMR1_CHID0_bit(PWM0);									//enable the interrupt from ADC 0, channel 0
	pwm_register_callback(&PWM_0, PWM_PERIOD_CB, pwm_0_callback);

	NVIC_EnableIRQ(PWM0_IRQn);
	NVIC_SetPriority(PWM0_IRQn, 2);
	
	NVIC_DisableIRQ(PWM1_IRQn);
	NVIC_ClearPendingIRQ(PWM0_IRQn);
}


//enable/disable pwm pins
//also sets periods and other variables which might not have been set at initialization
void pwm_enable_all(void){
	//note it's possible to eable individual channels, but not through these functions
	//functions from hal_pwm.h
	
	
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
	// the default function doesn't allow to set the PWM cycle on individual channels
	
	hri_pwm_write_CDTYUPD_reg(descr->device.hw, channel, duty_cycle);
	//hri_pwm_write_CPRDUPD_reg(device->hw, cfg->ch[i].index, period);			//period is constant; only altering duty cycle
}
