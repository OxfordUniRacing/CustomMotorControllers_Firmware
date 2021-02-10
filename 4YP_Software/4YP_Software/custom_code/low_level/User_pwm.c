/*
 * User_pwm.c
 *
 * Created: 11/01/2021 16:40:10
 *  Author: Ruzhev
 */ 


#include "User_pwm.h"

#include "User_Config.h"
#include "driver_init.h"




//enable/disable pwm pins
//also sets periods and other variables which might not have been set at initialization
void pwm_enable_all(void){
	//note it's possible to individual channels, but not through these functions
	//functions from hal_pwm.h
	
	//enable PWM0 and PWM1
	pwm_enable(&PWM_0);
	pwm_enable(&PWM_1);
		
	//set period and initial duty cycle
	//initial duty cycle = 0.5 * period <=> no current output for an H-bridge type driver
	pwm_set_parameters(&PWM_0, PWM_PERIOD, PWM_PERIOD>>1);
	pwm_set_parameters(&PWM_1  , PWM_PERIOD, PWM_PERIOD>>1);
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
	//uint8_t                i;
	//const struct _pwm_cfg *cfg;

	//ASSERT(device && (duty_cycle < period));

	//cfg = _pwm_get_cfg(device->hw);

	//for (i = 0; i < cfg->ch_num; i++) {										//not needed; we are altering one channel at a time
	hri_pwm_write_CDTYUPD_reg(descr->device.hw, channel, duty_cycle);
	//hri_pwm_write_CPRDUPD_reg(device->hw, cfg->ch[i].index, period);			//period is constant; only altering duty cycle
	//}
}
