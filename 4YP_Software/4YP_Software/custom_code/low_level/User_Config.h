/*
 * User_Config.h
 *
 * Created: 11/01/2021 15:27:23
 *  Author: Ruzhev
 */ 


#ifndef USER_CONFIG_H_
#define USER_CONFIG_H_

//include relevant macros
#include "driver_init.h"
#include "atmel_start_pins.h"

//--------------------------------------------------------------PWM------------------------------------------------------------------------------
//Clock is set to 150MHz by def (could go up to 300MHz)
//Required freq 15KHz
//Clock divide can be done through 1/2^n or by a single linear divider between 1/2, 1/3 and 1/255

//Using no dividers and setting period to 10 000 gives precisely 15KHz with resolution of ~14-bit
#define PWM_PERIOD 1000

/*
PWM functions look like this:
pwm_register_callback(&PWM_PHASE_A_B, PWM_PERIOD_CB, period_cb_PWM_PHASE_A_B);
pwm_enable(&PWM_PHASE_A_B);
pwm_set_parameters(&TEST_PWM_CHANNEL, 1024, 190); //1024 = period

//from <hpl_pwm.h>
void _pwm_set_param(struct _pwm_device *const device, const pwm_period_t period, const pwm_period_t duty_cycle)
{
	uint8_t                i;
	const struct _pwm_cfg *cfg;

	ASSERT(device && (duty_cycle < period));

	cfg = _pwm_get_cfg(device->hw);

	for (i = 0; i < cfg->ch_num; i++) {
		hri_pwm_write_CDTYUPD_reg(device->hw, cfg->ch[i].index, duty_cycle);
		hri_pwm_write_CPRDUPD_reg(device->hw, cfg->ch[i].index, period);
	}
}


The default pwm functions (shown above) work on all channels together. We need to control channels individually.
Therefore defining custom functions similar to the ones for ADC (see below)
*/
#define CONF_PWM_0_CHANNEL_0 0
#define CONF_PWM_0_CHANNEL_2 2
#define CONF_PWM_1_CHANNEL_0 0

#define PWM_PHASE_A &PWM_0, CONF_PWM_0_CHANNEL_0
#define PWM_PHASE_B &PWM_0, CONF_PWM_0_CHANNEL_2
#define PWM_PHASE_C &PWM_1  , CONF_PWM_1_CHANNEL_0




//--------------------------------------------------------------ADC------------------------------------------------------------------------------
//Clock settings are untouched - 150MHz clock with no divider; 12-bit resolution

/* 
ADC functions look like this:
adc_sync_enable_channel(&ADC_1, CONF_ADC_1_CHANNEL_0);
adc_sync_read_channel  (&ADC_1, CONF_ADC_1_CHANNEL_0, buffer_ch0, 2);

It is not convenient to have a reference of which signal was on which ADC and which channel.
Therefore a set of macros was made, corresponding to the correct ADC and channel. This way the function becomes:

adc_sync_enable_channel(ADC_CURRENT_A);
adc_sync_read_channel  (ADC_CURRENT_A, buffer_ch0, 2);
*/

#define ADC_CURRENT_A		&ADC_0, CONF_ADC_0_CHANNEL_8
#define ADC_CURRENT_B		&ADC_0, CONF_ADC_0_CHANNEL_2
#define ADC_CURRENT_C		&ADC_1, CONF_ADC_1_CHANNEL_1

#define ADC_SUPPL_VOLTAGE	&ADC_1, CONF_ADC_1_CHANNEL_6

#define ADC_TEMP_1			&ADC_1, CONF_ADC_1_CHANNEL_5
#define ADC_TEMP_2			&ADC_0, CONF_ADC_0_CHANNEL_6
//Temperature sensor 3 is currently disabled as it shares a port with USART1 which is used for EDBG Communication
//#define ADC_TEMP_3			&ADC_0, CONF_ADC_0_CHANNEL_1
#define ADC_TEMP_4			&ADC_0, CONF_ADC_0_CHANNEL_10
#define ADC_TEMP_5			&ADC_1, CONF_ADC_1_CHANNEL_0
#define ADC_TEMP_6			&ADC_0, CONF_ADC_0_CHANNEL_5
#define ADC_TEMP_MOTOR		&ADC_0, CONF_ADC_0_CHANNEL_0



//--------------------------------------------------------------ENCODER------------------------------------------------------------------------------

#define ENCODER_STEPS 4096
#define ENCODER_LAXIS_OFFSET 123


#endif /* USER_CONFIG_H_ */