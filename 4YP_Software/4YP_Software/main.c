#include <atmel_start.h>

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	/* Replace with your application code */
	while (1) {
		
		gpio_set_pin_level(PIN_USER_LED, false);
		//pwm_set_duty(PWM_PHASE_A, 8000);
		delay_ms(2000);
		printf("heya\n");
		
		gpio_set_pin_level(PIN_USER_LED, true);
		//pwm_set_duty(PWM_PHASE_A, 3000);
		delay_ms(2000);
	}
}
