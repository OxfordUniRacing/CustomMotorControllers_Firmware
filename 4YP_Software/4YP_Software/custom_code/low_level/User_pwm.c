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

void pwm_deadtime_init(void){													//Check section 51.6.2.5 Dead-Time Generator in datasheet
	/*
		Current inplementation of the deadtime includes adding the following code to _pwm_init() in hpl_pwm.c
				hri_pwmchnum_set_CMR_DTE_bit((void *) &((Pwm *)hw)->PwmChNum[ch->index]);
				hri_pwm_set_DT_DTH_bf(hw, ch->index, 10);
				hri_pwm_set_DT_DTL_bf(hw, ch->index, 10);
	*/
	
	/*
	*((unsigned int *)0x40020200) |= (1<<16);									//Set dead time enable to 1 in all the PWM 0 registers
	*((unsigned int *)0x40020220) |= (1<<16);
	*((unsigned int *)0x40020240) |= (1<<16);
	*((unsigned int *)0x40020260) |= (1<<16);
	
	*((unsigned int *)0x4005C200) |= (1<<16);									//Set dead time enable to 1 in all the PWM 1 registers
	*((unsigned int *)0x40020220) |= (1<<16);
	*((unsigned int *)0x40020240) |= (1<<16);
	*((unsigned int *)0x40020260) |= (1<<16);
	*/
	/*
	int8_t                      i;
	const struct _pwm_cfg *     cfg;
	const struct _pwm_ch_cfg *  ch;
	const struct _pwm_comp_cfg *comp;

	cfg = _pwm_get_cfg(PWM0);

	//device->hw = hw;
	

	
	for (i = 0; i < cfg->ch_num; i++) {
		ch = cfg->ch + i;
		hri_pwm_write_CMR_reg(PWM0, ch->index, ch->mode);

	}
	PWM_0->device->hw;
	*/
	//Pwm * pwminit = PWM0;
	
	//hri_pwmchnum_set_CMR_DTE_bit((Pwm *)PWM0->PwmChNum[0]);											//Set dead time enable to 1 in all the PWM 1 registers
	//hri_pwmchnum_set_CMR_DTE_bit(PWM0);											//Set dead time enable to 1 in all the PWM 0 registers
	
	//int DT = 3;																	//Dead time of 3 (for 3kHz (6MHz) corresponds to 0.5us)
	//PWM_1->device->hw
	//hri_pwmchnum_write_DT_DTH_bf(PWM0, DT);										//Set high dead time for high side of PWM0
	//hri_pwmchnum_write_DT_DTL_bf(PWM0, DT);										//Set high dead time for low side of PWM0
	//hri_pwmchnum_write_DT_DTH_bf(PWM1, DT);										//Set high dead time for high side of PWM1
	//hri_pwmchnum_write_DT_DTL_bf(PWM1, DT);										//Set high dead time for low side of PWM1
}
