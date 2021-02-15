/*
 * User_pwm.h
 *
 * Created: 11/01/2021 16:31:21
 *  Author: Ruzhev
 */ 


#ifndef USER_PWM_H_
#define USER_PWM_H_

#include <hal_pwm.h>

//enable/disable pwm pins
//also sets periods and other variables which might not have been set at initialization
void pwm_enable_all(void);
void pwm_disable_all(void);
void pwm_deadtime_init(void);

//sets individual channel pwm duty cycle
void pwm_set_duty(struct  pwm_descriptor * const descr, const uint8_t channel, const pwm_period_t duty_cycle);





#endif /* USER_PWM_H_ */