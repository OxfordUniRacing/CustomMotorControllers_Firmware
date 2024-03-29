/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_examples.h"
#include "driver_init.h"
#include "utils.h"

static void convert_cb_ADC_0_CHANNEL_0(const struct adc_async_descriptor *const descr, const uint8_t channel)
{
}

static void convert_cb_ADC_0_CHANNEL_2(const struct adc_async_descriptor *const descr, const uint8_t channel)
{
}

static void convert_cb_ADC_0_CHANNEL_5(const struct adc_async_descriptor *const descr, const uint8_t channel)
{
}

static void convert_cb_ADC_0_CHANNEL_6(const struct adc_async_descriptor *const descr, const uint8_t channel)
{
}

static void convert_cb_ADC_0_CHANNEL_8(const struct adc_async_descriptor *const descr, const uint8_t channel)
{
}

static void convert_cb_ADC_0_CHANNEL_10(const struct adc_async_descriptor *const descr, const uint8_t channel)
{
}

/**
 * Example of using ADC_0 to generate waveform.
 */
void ADC_0_example(void)
{
	adc_async_register_callback(&ADC_0, CONF_ADC_0_CHANNEL_0, ADC_ASYNC_CONVERT_CB, convert_cb_ADC_0_CHANNEL_0);
	adc_async_register_callback(&ADC_0, CONF_ADC_0_CHANNEL_2, ADC_ASYNC_CONVERT_CB, convert_cb_ADC_0_CHANNEL_2);
	adc_async_register_callback(&ADC_0, CONF_ADC_0_CHANNEL_5, ADC_ASYNC_CONVERT_CB, convert_cb_ADC_0_CHANNEL_5);
	adc_async_register_callback(&ADC_0, CONF_ADC_0_CHANNEL_6, ADC_ASYNC_CONVERT_CB, convert_cb_ADC_0_CHANNEL_6);
	adc_async_register_callback(&ADC_0, CONF_ADC_0_CHANNEL_8, ADC_ASYNC_CONVERT_CB, convert_cb_ADC_0_CHANNEL_8);
	adc_async_register_callback(&ADC_0, CONF_ADC_0_CHANNEL_10, ADC_ASYNC_CONVERT_CB, convert_cb_ADC_0_CHANNEL_10);
	adc_async_enable_channel(&ADC_0, CONF_ADC_0_CHANNEL_0);
	adc_async_enable_channel(&ADC_0, CONF_ADC_0_CHANNEL_2);
	adc_async_enable_channel(&ADC_0, CONF_ADC_0_CHANNEL_5);
	adc_async_enable_channel(&ADC_0, CONF_ADC_0_CHANNEL_6);
	adc_async_enable_channel(&ADC_0, CONF_ADC_0_CHANNEL_8);
	adc_async_enable_channel(&ADC_0, CONF_ADC_0_CHANNEL_10);
	adc_async_start_conversion(&ADC_0);
}

static void convert_cb_ADC_1_CHANNEL_0(const struct adc_async_descriptor *const descr, const uint8_t channel)
{
}

static void convert_cb_ADC_1_CHANNEL_1(const struct adc_async_descriptor *const descr, const uint8_t channel)
{
}

static void convert_cb_ADC_1_CHANNEL_5(const struct adc_async_descriptor *const descr, const uint8_t channel)
{
}

static void convert_cb_ADC_1_CHANNEL_6(const struct adc_async_descriptor *const descr, const uint8_t channel)
{
}

/**
 * Example of using ADC_1 to generate waveform.
 */
void ADC_1_example(void)
{
	adc_async_register_callback(&ADC_1, CONF_ADC_1_CHANNEL_0, ADC_ASYNC_CONVERT_CB, convert_cb_ADC_1_CHANNEL_0);
	adc_async_register_callback(&ADC_1, CONF_ADC_1_CHANNEL_1, ADC_ASYNC_CONVERT_CB, convert_cb_ADC_1_CHANNEL_1);
	adc_async_register_callback(&ADC_1, CONF_ADC_1_CHANNEL_5, ADC_ASYNC_CONVERT_CB, convert_cb_ADC_1_CHANNEL_5);
	adc_async_register_callback(&ADC_1, CONF_ADC_1_CHANNEL_6, ADC_ASYNC_CONVERT_CB, convert_cb_ADC_1_CHANNEL_6);
	adc_async_enable_channel(&ADC_1, CONF_ADC_1_CHANNEL_0);
	adc_async_enable_channel(&ADC_1, CONF_ADC_1_CHANNEL_1);
	adc_async_enable_channel(&ADC_1, CONF_ADC_1_CHANNEL_5);
	adc_async_enable_channel(&ADC_1, CONF_ADC_1_CHANNEL_6);
	adc_async_start_conversion(&ADC_1);
}

static void button_on_PD19_pressed(void)
{
}

/**
 * Example of using EXTERNAL_IRQ_D
 */
void EXTERNAL_IRQ_D_example(void)
{
	ext_irq_register(PIO_PD19_IDX, button_on_PD19_pressed);
}

static void button_on_PB13_pressed(void)
{
}

/**
 * Example of using EXTERNAL_IRQ_B
 */
void EXTERNAL_IRQ_B_example(void)
{
	ext_irq_register(PIO_PB13_IDX, button_on_PB13_pressed);
}

static void button_on_PA2_pressed(void)
{
}
static void button_on_PA5_pressed(void)
{
}

/**
 * Example of using EXTERNAL_IRQ_A
 */
void EXTERNAL_IRQ_A_example(void)
{
	ext_irq_register(PIO_PA2_IDX, button_on_PA2_pressed);
	ext_irq_register(PIO_PA5_IDX, button_on_PA5_pressed);
}

static void period_cb_PWM_0(const struct pwm_descriptor *const descr)
{
	/* period interrupt */
}
/**
 * Example of using PWM_0.
 */
void PWM_0_example(void)
{
	pwm_register_callback(&PWM_0, PWM_PERIOD_CB, period_cb_PWM_0);
	pwm_enable(&PWM_0);
}

static void period_cb_PWM_1(const struct pwm_descriptor *const descr)
{
	/* period interrupt */
}
/**
 * Example of using PWM_1.
 */
void PWM_1_example(void)
{
	pwm_register_callback(&PWM_1, PWM_PERIOD_CB, period_cb_PWM_1);
	pwm_enable(&PWM_1);
}

/**
 * Example of using ENCODER_A.
 */
static struct timer_task ENCODER_A_task1, ENCODER_A_task2;

static void ENCODER_A_task1_cb(const struct timer_task *const timer_task)
{
}

static void ENCODER_A_task2_cb(const struct timer_task *const timer_task)
{
}

void ENCODER_A_example(void)
{
	ENCODER_A_task1.interval = 100;
	ENCODER_A_task1.cb       = ENCODER_A_task1_cb;
	ENCODER_A_task1.mode     = TIMER_TASK_REPEAT;
	ENCODER_A_task2.interval = 200;
	ENCODER_A_task2.cb       = ENCODER_A_task2_cb;
	ENCODER_A_task2.mode     = TIMER_TASK_REPEAT;

	timer_add_task(&ENCODER_A, &ENCODER_A_task1);
	timer_add_task(&ENCODER_A, &ENCODER_A_task2);
	timer_start(&ENCODER_A);
}

/**
 * Example of using ENCODER_B.
 */
static struct timer_task ENCODER_B_task1, ENCODER_B_task2;

static void ENCODER_B_task1_cb(const struct timer_task *const timer_task)
{
}

static void ENCODER_B_task2_cb(const struct timer_task *const timer_task)
{
}

void ENCODER_B_example(void)
{
	ENCODER_B_task1.interval = 100;
	ENCODER_B_task1.cb       = ENCODER_B_task1_cb;
	ENCODER_B_task1.mode     = TIMER_TASK_REPEAT;
	ENCODER_B_task2.interval = 200;
	ENCODER_B_task2.cb       = ENCODER_B_task2_cb;
	ENCODER_B_task2.mode     = TIMER_TASK_REPEAT;

	timer_add_task(&ENCODER_B, &ENCODER_B_task1);
	timer_add_task(&ENCODER_B, &ENCODER_B_task2);
	timer_start(&ENCODER_B);
}

void delay_example(void)
{
	delay_ms(5000);
}

/**
 * Example of using EDBG_COM to write "Hello World" using the IO abstraction.
 */
void EDBG_COM_example(void)
{
	struct io_descriptor *io;
	usart_sync_get_io_descriptor(&EDBG_COM, &io);
	usart_sync_enable(&EDBG_COM);

	io_write(io, (uint8_t *)"Hello World!", 12);
}

void CAN_1_tx_callback(struct can_async_descriptor *const descr)
{
	(void)descr;
}
void CAN_1_rx_callback(struct can_async_descriptor *const descr)
{
	struct can_message msg;
	uint8_t            data[64];
	msg.data = data;
	can_async_read(descr, &msg);
	return;
}

/**
 * Example of using CAN_1 to Encrypt/Decrypt datas.
 */
void CAN_1_example(void)
{
	struct can_message msg;
	struct can_filter  filter;
	uint8_t            send_data[4];
	send_data[0] = 0x00;
	send_data[1] = 0x01;
	send_data[2] = 0x02;
	send_data[3] = 0x03;

	msg.id   = 0x45A;
	msg.type = CAN_TYPE_DATA;
	msg.data = send_data;
	msg.len  = 4;
	msg.fmt  = CAN_FMT_STDID;
	can_async_register_callback(&CAN_1, CAN_ASYNC_TX_CB, (FUNC_PTR)CAN_1_tx_callback);
	can_async_enable(&CAN_1);
	/**
	 * CAN_1_tx_callback callback should be invoked after call
	 * can_async_write, and remote device should recieve message with ID=0x45A
	 */
	can_async_write(&CAN_1, &msg);

	msg.id  = 0x100000A5;
	msg.fmt = CAN_FMT_EXTID;
	/**
	 * remote device should recieve message with ID=0x100000A5
	 */
	can_async_write(&CAN_1, &msg);

	/**
	 * CAN_1_rx_callback callback should be invoked after call
	 * can_async_set_filter and remote device send CAN Message with the same
	 * content as the filter.
	 */
	can_async_register_callback(&CAN_1, CAN_ASYNC_RX_CB, (FUNC_PTR)CAN_1_rx_callback);
	filter.id   = 0x469;
	filter.mask = 0;
	can_async_set_filter(&CAN_1, 0, CAN_FMT_STDID, &filter);

	filter.id   = 0x10000096;
	filter.mask = 0;
	can_async_set_filter(&CAN_1, 1, CAN_FMT_EXTID, &filter);
}
