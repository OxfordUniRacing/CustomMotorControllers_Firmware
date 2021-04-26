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
#include "Time_Tester.h"

int counter = 0;

void pwm_0_callback(const struct pwm_descriptor *const descr){
	
	gather_control_data();
	
	/*
	counter ++;
	if (counter > 15000){
		counter = 0;
		printf("PWM Interrupt \n");
	}
	*/
	
	//for timing diagram
	time_record_timestamp();
}


// void pwm_1_callback(const struct pwm_descriptor *const descr){
// 	
// 	//for timing diagram
// 	//time_record_timestamp();
// }


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
	
	
	//set comparison value for comparison unit. This should be done by the init function, however something is not working there; comparrison value = 1
	//comparison channel was selected to 0
	hri_pwm_set_CMPV_reg(PWM0, PWM_COMPARISON_UNIT_CHANNEL, PWM_CMPV_CV(CONF_PWM_0_CV0) );
	hri_pwm_set_CMPV_reg(PWM1, PWM_COMPARISON_UNIT_CHANNEL, PWM_CMPV_CV(CONF_PWM_1_CV0) );
	
	
	//we want interrupt from one of the PWMs so that we can start the control loop
	//interrupt on PWM 0, channel 0 is enabled ; on PWM 1 is disabled
	hri_pwm_set_IMR1_CHID0_bit(PWM0);									//enable the interrupt from PWM 0, channel 0
	pwm_register_callback(&PWM_0, PWM_PERIOD_CB, pwm_0_callback);
	

	
	NVIC_EnableIRQ(PWM0_IRQn);
	NVIC_SetPriority(PWM0_IRQn, IRQ_PRIORITY_PWM);
	
	NVIC_DisableIRQ(PWM1_IRQn);
	NVIC_ClearPendingIRQ(PWM1_IRQn);
	
	
	
	//for timing testing
// 	hri_pwm_set_IMR1_CHID0_bit(PWM1);									//enable the interrupt from PWM 0, channel 0
// 	pwm_register_callback(&PWM_1, PWM_PERIOD_CB, pwm_1_callback);
// 	NVIC_EnableIRQ(PWM1_IRQn);
// 	NVIC_SetPriority(PWM1_IRQn, IRQ_PRIORITY_PWM);
}


//enable/disable pwm pins
//also sets periods and other variables which might not have been set at initialization
void pwm_enable_all(void){
	//note it's possible to eable individual channels, but not through these functions
	//functions from hal_pwm.h
	
	
	//enable comparison unit. This triggers the event line for the ADCs
	hri_pwm_set_CMPM_reg(PWM0, PWM_COMPARISON_UNIT_CHANNEL, PWM_CMPM_CEN_Msk);
	hri_pwm_set_CMPM_reg(PWM1, PWM_COMPARISON_UNIT_CHANNEL, PWM_CMPM_CEN_Msk);
	
	
	//enable PWM0 and PWM1
	pwm_enable(&PWM_0);
	pwm_enable(&PWM_1);
		
	//set period and initial duty cycle
	//initial duty cycle = 0.5 * period <=> no current output for an H-bridge type driver
	pwm_set_parameters(&PWM_0, PWM_PERIOD, PWM_PERIOD-1);
	pwm_set_parameters(&PWM_1, PWM_PERIOD, PWM_PERIOD-1);
	
	
	
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
