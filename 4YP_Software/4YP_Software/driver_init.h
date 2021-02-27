/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#ifndef DRIVER_INIT_H_INCLUDED
#define DRIVER_INIT_H_INCLUDED

#include "atmel_start_pins.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <hal_atomic.h>
#include <hal_delay.h>
#include <hal_gpio.h>
#include <hal_init.h>
#include <hal_io.h>
#include <hal_sleep.h>

#include <hal_adc_async.h>
#include <hal_adc_async.h>
#include <hal_ext_irq.h>
#include <hal_ext_irq.h>

#include <hal_pwm.h>

#include <hal_pwm.h>
#include <hal_timer.h>
#include <hal_timer.h>

#include <hal_delay.h>

#include <hal_usart_sync.h>

#include <hal_can_async.h>

extern struct adc_async_descriptor ADC_0;

/* The enabled channel for ADC */
#define CONF_ADC_0_CHANNEL_0 0

#define CONF_ADC_0_CHANNEL_2 2

#define CONF_ADC_0_CHANNEL_5 5
#define CONF_ADC_0_CHANNEL_6 6

#define CONF_ADC_0_CHANNEL_8 8

#define CONF_ADC_0_CHANNEL_10 10

extern struct adc_async_descriptor ADC_1;

/* The enabled channel for ADC */
#define CONF_ADC_1_CHANNEL_0 0
#define CONF_ADC_1_CHANNEL_1 1

#define CONF_ADC_1_CHANNEL_5 5
#define CONF_ADC_1_CHANNEL_6 6

extern struct pwm_descriptor PWM_0;

extern struct pwm_descriptor   PWM_1;
extern struct timer_descriptor ENCODER_A;
extern struct timer_descriptor ENCODER_B;

extern struct usart_sync_descriptor EDBG_COM;
extern struct can_async_descriptor  CAN_1;

void PWM_0_PORT_init(void);
void PWM_0_CLOCK_init(void);
void PWM_0_init(void);

void PWM_1_PORT_init(void);
void PWM_1_CLOCK_init(void);
void PWM_1_init(void);

void delay_driver_init(void);

void EDBG_COM_PORT_init(void);
void EDBG_COM_CLOCK_init(void);
void EDBG_COM_init(void);

void CAN_1_PORT_init(void);
void CAN_1_CLOCK_init(void);
void CAN_1_init(void);
void CAN_1_example(void);

/**
 * \brief Perform system initialization, initialize pins and clocks for
 * peripherals
 */
void system_init(void);

#ifdef __cplusplus
}
#endif
#endif // DRIVER_INIT_H_INCLUDED
