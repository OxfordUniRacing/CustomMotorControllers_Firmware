/*
 * User_adc.c
 *
 * Created: 11/01/2021 17:27:20
 *  Author: Ruzhev
 */ 

#include "User_adc.h"
#include "driver_init.h"
#include "User_Config.h"


/*
adc_sync_enable_channel(struct adc_sync_descriptor *const descr, const uint8_t channel);
adc_sync_read_channel(struct adc_sync_descriptor *const descr, const uint8_t channel, uint8_t *const buffer,
const uint16_t length);
*/


//enables/disables all AFECs
void adc_enable_all(void){
	//note that it is possible to enable individual channels
	//functions from hal_adc_sync.h
	adc_sync_enable_channel(ADC_CURRENT_A);
	adc_sync_enable_channel(ADC_CURRENT_B);
	adc_sync_enable_channel(ADC_CURRENT_C);
	
	adc_sync_enable_channel(ADC_SUPPL_VOLTAGE);
	
	adc_sync_enable_channel(ADC_TEMP_1);
	adc_sync_enable_channel(ADC_TEMP_2);
	//adc_sync_enable_channel(ADC_TEMP_3);
	adc_sync_enable_channel(ADC_TEMP_4);
	adc_sync_enable_channel(ADC_TEMP_5);
	adc_sync_enable_channel(ADC_TEMP_6);
	adc_sync_enable_channel(ADC_TEMP_MOTOR);
}

void adc_disable_all(void){
	//functions from hal_adc_sync.h
	adc_sync_disable_channel(ADC_CURRENT_A);
	adc_sync_disable_channel(ADC_CURRENT_B);
	adc_sync_disable_channel(ADC_CURRENT_C);
		
	adc_sync_disable_channel(ADC_SUPPL_VOLTAGE);
	
	adc_sync_disable_channel(ADC_TEMP_1);
	adc_sync_disable_channel(ADC_TEMP_2);
	//adc_sync_disable_channel(ADC_TEMP_3);
	adc_sync_disable_channel(ADC_TEMP_4);
	adc_sync_disable_channel(ADC_TEMP_5);
	adc_sync_disable_channel(ADC_TEMP_6);
	adc_sync_disable_channel(ADC_TEMP_MOTOR);
}

//reads ADC value
uint16_t adc_read(struct adc_sync_descriptor *const descr, const uint8_t channel){
	
	uint16_t temp;
	
	adc_sync_read_channel(descr, channel, (uint8_t *)&temp, 2);
	
	return temp;
}
