/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_init.h"
#include <hal_init.h>
#include <hpl_pmc.h>
#include <peripheral_clk_config.h>
#include <utils.h>
#include <hpl_pwm_base.h>

#include <hpl_pwm_base.h>

#include <hpl_tc.h>

#include <hpl_tc.h>

#include <hpl_usart_base.h>

struct timer_descriptor     ENCODER_A;
struct timer_descriptor     ENCODER_B;
struct can_async_descriptor CAN_1;

struct adc_sync_descriptor ADC_0;

struct adc_sync_descriptor ADC_1;

struct pwm_descriptor PWM_0;

struct pwm_descriptor PWM_1;

struct usart_sync_descriptor EDBG_COM;

void ADC_0_PORT_init(void)
{

	gpio_set_pin_function(PIN_ADC_TEMP_MOTOR, GPIO_PIN_FUNCTION_OFF);

	gpio_set_pin_function(PIN_ADC_CURR_B, GPIO_PIN_FUNCTION_OFF);

	gpio_set_pin_function(PIN_ADC_TEMP_6, GPIO_PIN_FUNCTION_OFF);

	gpio_set_pin_function(PIN_ADC_TEMP_2, GPIO_PIN_FUNCTION_OFF);

	gpio_set_pin_function(PIN_ADC_CURR_A, GPIO_PIN_FUNCTION_OFF);

	gpio_set_pin_function(PIN_ADC_TEMP_4, GPIO_PIN_FUNCTION_OFF);
}

void ADC_0_CLOCK_init(void)
{

	_pmc_enable_periph_clock(ID_AFEC0);
}

void ADC_0_init(void)
{
	ADC_0_CLOCK_init();
	ADC_0_PORT_init();
	adc_sync_init(&ADC_0, AFEC0, (void *)NULL);
}

void ADC_1_PORT_init(void)
{

	gpio_set_pin_function(PIN_ADC_TEMP_5, GPIO_PIN_FUNCTION_OFF);

	gpio_set_pin_function(PIN_ADC_CURR_C, GPIO_PIN_FUNCTION_OFF);

	gpio_set_pin_function(PIN_ADC_TEMP_1, GPIO_PIN_FUNCTION_OFF);

	gpio_set_pin_function(PIN_ADC_SUPPL_VOLTAGE, GPIO_PIN_FUNCTION_OFF);
}

void ADC_1_CLOCK_init(void)
{

	_pmc_enable_periph_clock(ID_AFEC1);
}

void ADC_1_init(void)
{
	ADC_1_CLOCK_init();
	ADC_1_PORT_init();
	adc_sync_init(&ADC_1, AFEC1, (void *)NULL);
}

void EXTERNAL_IRQ_D_init(void)
{

	// Set pin direction to input
	gpio_set_pin_direction(PIN_GPIO_POS_3, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(PIN_GPIO_POS_3,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PIN_GPIO_POS_3, GPIO_PIN_FUNCTION_OFF);
}

void EXTERNAL_IRQ_A_init(void)
{

	// Set pin direction to input
	gpio_set_pin_direction(PIN_GPIO_POS_2, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(PIN_GPIO_POS_2,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PIN_GPIO_POS_2, GPIO_PIN_FUNCTION_OFF);

	// Set pin direction to input
	gpio_set_pin_direction(PIN_GPIO_POS_1, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(PIN_GPIO_POS_1,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PIN_GPIO_POS_1, GPIO_PIN_FUNCTION_OFF);

	// Set pin direction to input
	gpio_set_pin_direction(PIN_ENCODER_Z, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(PIN_ENCODER_Z,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PIN_ENCODER_Z, GPIO_PIN_FUNCTION_OFF);
}

void PWM_0_PORT_init(void)
{

	gpio_set_pin_function(PIN_PWM_PHASE_A_HIGH, MUX_PA23B_PWM0_PWMH0);

	gpio_set_pin_function(PIN_PWM_PHASE_B_HIGH, MUX_PA25B_PWM0_PWMH2);

	gpio_set_pin_function(PIN_PWM_PHASE_A_LOW, MUX_PA1A_PWM0_PWML0);

	gpio_set_pin_function(PIN_PWM_PHASE_B_LOW, MUX_PA30A_PWM0_PWML2);
}

void PWM_0_CLOCK_init(void)
{
	_pmc_enable_periph_clock(ID_PWM0);
}

void PWM_0_init(void)
{
	PWM_0_CLOCK_init();
	PWM_0_PORT_init();
	pwm_init(&PWM_0, PWM0, _pwm_get_pwm());
}

void PWM_1_PORT_init(void)
{

	gpio_set_pin_function(PIN_PWM_PHASE_C_HIGH, MUX_PA12C_PWM1_PWMH0);

	gpio_set_pin_function(PIN_PWM_PHASE_C_LOW, MUX_PD0B_PWM1_PWML0);
}

void PWM_1_CLOCK_init(void)
{
	_pmc_enable_periph_clock(ID_PWM1);
}

void PWM_1_init(void)
{
	PWM_1_CLOCK_init();
	PWM_1_PORT_init();
	pwm_init(&PWM_1, PWM1, _pwm_get_pwm());
}

void ENCODER_A_PORT_init(void)
{

	gpio_set_pin_function(PIN_ENCODER_A, MUX_PA29B_TC0_TCLK2);
}
/**
 * \brief Timer initialization function
 *
 * Enables Timer peripheral, clocks and initializes Timer driver
 */
static void ENCODER_A_init(void)
{
	_pmc_enable_periph_clock(ID_TC0_CHANNEL0);
	ENCODER_A_PORT_init();
	timer_init(&ENCODER_A, TC0, _tc_get_timer());
}

void ENCODER_B_PORT_init(void)
{

	gpio_set_pin_function(PIN_ENCODER_B, MUX_PD24C_TC3_TCLK11);
}
/**
 * \brief Timer initialization function
 *
 * Enables Timer peripheral, clocks and initializes Timer driver
 */
static void ENCODER_B_init(void)
{
	_pmc_enable_periph_clock(ID_TC3_CHANNEL0);
	ENCODER_B_PORT_init();
	timer_init(&ENCODER_B, TC3, _tc_get_timer());
}

void delay_driver_init(void)
{
	delay_init(SysTick);
}

void EDBG_COM_PORT_init(void)
{

	gpio_set_pin_function(PIN_EDBG_COM_RX, MUX_PA21A_USART1_RXD1);

	gpio_set_pin_function(PIN_EDBG_COM_TX, MUX_PB4D_USART1_TXD1);
}

void EDBG_COM_CLOCK_init(void)
{
	_pmc_enable_periph_clock(ID_USART1);
}

void EDBG_COM_init(void)
{
	EDBG_COM_CLOCK_init();
	EDBG_COM_PORT_init();
	usart_sync_init(&EDBG_COM, USART1, _usart_get_usart_sync());
}

/**
 * \brief MCAN Clock initialization function
 *
 * Enables register interface and peripheral clock
 */
void CAN_1_CLOCK_init()
{
	_pmc_enable_periph_clock(ID_MCAN1);
}

/**
 * \brief MCAN pinmux initialization function
 *
 * Set each required pin to MCAN functionality
 */
void CAN_1_PORT_init(void)
{

	gpio_set_pin_function(PIN_CAN_RX, MUX_PC12C_MCAN1_CANRX1);

	gpio_set_pin_function(PIN_CAN_TX, MUX_PC14C_MCAN1_CANTX1);
}
/**
 * \brief CAN initialization function
 *
 * Enables CAN peripheral, clocks and initializes CAN driver
 */
void CAN_1_init(void)
{
	CAN_1_CLOCK_init();
	CAN_1_PORT_init();
	can_async_init(&CAN_1, MCAN1);
}

void system_init(void)
{
	init_mcu();

	_pmc_enable_periph_clock(ID_PIOA);

	_pmc_enable_periph_clock(ID_PIOC);

	_pmc_enable_periph_clock(ID_PIOD);

	/* Disable Watchdog */
	hri_wdt_set_MR_WDDIS_bit(WDT);

	/* GPIO on PA11 */

	// Set pin direction to input
	gpio_set_pin_direction(PIN_USER_BUTTON, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(PIN_USER_BUTTON,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_UP);

	gpio_set_pin_function(PIN_USER_BUTTON, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PA18 */

	gpio_set_pin_level(PIN_GPIO_DCDC_ON_OFF,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(PIN_GPIO_DCDC_ON_OFF, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(PIN_GPIO_DCDC_ON_OFF, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PA28 */

	gpio_set_pin_level(PIN_GPIO_CAN_STNDBY,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(PIN_GPIO_CAN_STNDBY, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(PIN_GPIO_CAN_STNDBY, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PC8 */

	gpio_set_pin_level(PIN_USER_LED,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(PIN_USER_LED, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(PIN_USER_LED, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PC9 */

	gpio_set_pin_level(PIN_GPIO_11,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(PIN_GPIO_11, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(PIN_GPIO_11, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PD18 */

	gpio_set_pin_level(PIN_GPIO_4,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(PIN_GPIO_4, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(PIN_GPIO_4, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PD27 */

	gpio_set_pin_level(PIN_GPIO_5,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(PIN_GPIO_5, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(PIN_GPIO_5, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PD28 */

	gpio_set_pin_level(PIN_GPIO_6,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(PIN_GPIO_6, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(PIN_GPIO_6, GPIO_PIN_FUNCTION_OFF);

	ADC_0_init();

	ADC_1_init();
	EXTERNAL_IRQ_D_init();
	EXTERNAL_IRQ_A_init();

	PWM_0_init();

	PWM_1_init();
	ENCODER_A_init();
	ENCODER_B_init();

	delay_driver_init();

	EDBG_COM_init();

	//CAN_1_init();

	ext_irq_init();
}
