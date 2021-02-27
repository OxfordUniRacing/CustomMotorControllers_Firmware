/*
 * User_adc.c
 *
 * Created: 11/01/2021 17:27:20
 *  Author: Ruzhev
 */ 

#include "User_adc.h"
#include "driver_init.h"
#include "User_Config.h"
#include "hpl_dma.h"







//buffers for the DMA to put the output of the ADCs
static uint32_t dma_adc_0_buff[ADC_0_NUM_ACTIVE_CHANNELS];
static uint32_t dma_adc_1_buff[ADC_1_NUM_ACTIVE_CHANNELS];

//callback functions for when transactions are complete
static void dma_adc_0_callback(struct _dma_resource *resource){
	//just for testing
	printf("interrupt - ADC 0 - %i %i %i %i %i %i  \n", (int)dma_adc_0_buff[0],(int)dma_adc_0_buff[1],(int)dma_adc_0_buff[2],(int)dma_adc_0_buff[3],\
		(int)dma_adc_0_buff[4],(int)dma_adc_0_buff[5]);
}

static void dma_adc_1_callback(struct _dma_resource *resource){
	//just for testing
	printf("interrupt - ADC 1 - %i %i %i %i  \n", (int)dma_adc_1_buff[0],(int)dma_adc_1_buff[1],(int)dma_adc_1_buff[2],(int)dma_adc_1_buff[3]);
}

void dma_adc_init(void){
	//initialises the standard values for the DMA controller
	
	
	
	
	//channel 0 for ADC 0
	
	//source address = AFEC 0->LCDR
	//_dma_set_source_address(0,(void *) ((Afec *)((&ADC_0)->device.hw) + AFEC_LCDR_OFFSET));
	_dma_set_source_address			(DMA_ADC_0_CHANNEL,(void *)0x4003C020);	//there is a more adequate way of defining this, but I coundt get it to work
	_dma_set_destination_address	(DMA_ADC_0_CHANNEL, dma_adc_0_buff);
	_dma_set_data_amount			(DMA_ADC_0_CHANNEL , ADC_0_SIZE_OF_GENERATED_DATA);
	
	//set callback function
	struct _dma_resource **res0;
	_dma_get_channel_resource(res0, DMA_ADC_0_CHANNEL);
	(*res0)->dma_cb.transfer_done = dma_adc_0_callback;
	
	//enable interrupt on successful transfer
	_dma_set_irq_state(DMA_ADC_0_CHANNEL, DMA_TRANSFER_COMPLETE_CB, true);
	//not handling errors at this point in time
	//_dma_set_irq_state(DMA_ADC_0_CHANNEL,DMA_TRANSFER_ERROR_CB,true);
	
	
	
	
	//channel 0 for ADC 0
	
	//source address = AFEC 1->LCDR
	//_dma_set_source_address(0,(void *) ((Afec *)((&ADC_0)->device.hw) + AFEC_LCDR_OFFSET));
	_dma_set_source_address			(DMA_ADC_1_CHANNEL,(void *)0x40064020);	//there is a more adequate way of defining this, but I coundt get it to work
	_dma_set_destination_address	(DMA_ADC_1_CHANNEL, dma_adc_1_buff);
	_dma_set_data_amount			(DMA_ADC_1_CHANNEL , ADC_1_SIZE_OF_GENERATED_DATA);
	
	//set callback function
	struct _dma_resource **res1;
	_dma_get_channel_resource(res1, DMA_ADC_1_CHANNEL);
	(*res1)->dma_cb.transfer_done = dma_adc_1_callback;
	
	//enable interrupt on successful transfer
	_dma_set_irq_state(DMA_ADC_1_CHANNEL, DMA_TRANSFER_COMPLETE_CB, true);
	//not handling errors at this point in time
	//_dma_set_irq_state(DMA_ADC_1_CHANNEL,DMA_TRANSFER_ERROR_CB,true);
		
	
}

void dma_adc_0_enable_for_one_transaction(void){
	//reset to base address
	_dma_set_destination_address	(DMA_ADC_0_CHANNEL, dma_adc_0_buff);
	//set how much data to move (in bytes)
	_dma_set_data_amount			(DMA_ADC_0_CHANNEL, ADC_0_SIZE_OF_GENERATED_DATA);
	//enable for one transaction
	_dma_enable_transaction			(DMA_ADC_0_CHANNEL, true);
}

void dma_adc_1_enable_for_one_transaction(void){
	//reset to base address
	_dma_set_destination_address	(DMA_ADC_1_CHANNEL, dma_adc_1_buff);
	//set how much data to move (in bytes)
	_dma_set_data_amount			(DMA_ADC_1_CHANNEL, ADC_1_SIZE_OF_GENERATED_DATA);
	//enable for one transaction
	_dma_enable_transaction			(DMA_ADC_1_CHANNEL, true);
}


/*
adc_sync_enable_channel(struct adc_sync_descriptor *const descr, const uint8_t channel);
adc_sync_read_channel(struct adc_sync_descriptor *const descr, const uint8_t channel, uint8_t *const buffer,
const uint16_t length);
*/


//enables/disables all AFECs
void adc_enable_all(void){
	//note that it is possible to enable individual channels
	//functions from hal_adc_sync.h
	adc_async_enable_channel(ADC_CURRENT_A);
	adc_async_enable_channel(ADC_CURRENT_B);
	adc_async_enable_channel(ADC_CURRENT_C);
	
	adc_async_enable_channel(ADC_SUPPL_VOLTAGE);
	
	adc_async_enable_channel(ADC_TEMP_1);
	adc_async_enable_channel(ADC_TEMP_2);
	//adc_sync_enable_channel(ADC_TEMP_3);
	adc_async_enable_channel(ADC_TEMP_4);
	adc_async_enable_channel(ADC_TEMP_5);
	adc_async_enable_channel(ADC_TEMP_6);
	adc_async_enable_channel(ADC_TEMP_MOTOR);
}

void adc_disable_all(void){
	//functions from hal_adc_sync.h
	adc_async_disable_channel(ADC_CURRENT_A);
	adc_async_disable_channel(ADC_CURRENT_B);
	adc_async_disable_channel(ADC_CURRENT_C);
		
	adc_async_disable_channel(ADC_SUPPL_VOLTAGE);
	
	adc_async_disable_channel(ADC_TEMP_1);
	adc_async_disable_channel(ADC_TEMP_2);
	//adc_sync_disable_channel(ADC_TEMP_3);
	adc_async_disable_channel(ADC_TEMP_4);
	adc_async_disable_channel(ADC_TEMP_5);
	adc_async_disable_channel(ADC_TEMP_6);
	adc_async_disable_channel(ADC_TEMP_MOTOR);
}

//reads ADC value
uint16_t adc_read(struct adc_async_descriptor *const descr, const uint8_t channel){
	
	uint16_t temp;
	
	adc_async_read_channel(descr, channel, (uint8_t *)&temp, 2);
	
	return temp;
}
