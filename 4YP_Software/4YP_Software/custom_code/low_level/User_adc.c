/*
 * User_adc.c
 *
 * Created: 11/01/2021 17:27:20
 *  Author: Ruzhev
 */ 

#include "User_adc.h"
#include "driver_init.h"
#include "hpl_dma.h"
#include <hal_adc_async.h>

#include "component/afec.h"





//arrays for passing on the values to the control functions
int raw_currents[3];
int raw_voltage;

//curr A,B,C are 0,1,2 and voltage is 3
static char ready_values = 0;
#define ALL_VALUES_READY (0b1111)


//callback functions for when transactions are complete
static void dma_adc_0_callback(struct _dma_resource *resource){
	//just for testing
	printf("interrupt - ADC 0 - %i %i %i %i %i %i  \n", (int)dma_adc_0_buff[0],(int)dma_adc_0_buff[1],(int)dma_adc_0_buff[2],(int)dma_adc_0_buff[3], (int)dma_adc_0_buff[4],(int)dma_adc_0_buff[5]);
	
	
	//go through the values that the DMA got and get the ones that we need (currents and bus voltage)
	for (int i =0; i<ADC_0_NUM_ACTIVE_CHANNELS; i++){
		
		switch((dma_adc_0_buff[i] & AFEC_LCDR_CHNB_Msk)){
			
			case AFEC_LCDR_CHNB(ADC_CURRENT_A_CHANNEL):
				raw_currents[0] = (int) (dma_adc_0_buff[i] & AFEC_LCDR_LDATA_Msk);
				ready_values |= (1<<0);
				break;
			
			case AFEC_LCDR_CHNB(ADC_CURRENT_B_CHANNEL):
				raw_currents[1] = (int) (dma_adc_0_buff[i] & AFEC_LCDR_LDATA_Msk);
				ready_values |= (1<<1);
				break;
			
			default:
				;
		}
	}
	
	//current conversion is done
	//enable the next conversion if continuous mode enabled
	if(is_dma_adc_0_continuous){
		dma_adc_0_enable_for_one_transaction();
	}
	
	
	
	if(ready_values == ALL_VALUES_READY){
		//means we have collected the data from all ADCs
		
		//we would need new values for next loop
		ready_values = 0;
		
		//launch control loop
		
		printf("Data collected, launching control loop from adc 0\n");
		printf("%i %i %i %i  \n", raw_voltage, raw_currents[0], raw_currents[1], raw_currents[2], raw_currents[3]);
	}
	
}

static void dma_adc_1_callback(struct _dma_resource *resource){
	//just for testing
	printf("interrupt - ADC 1 - %i %i %i %i  \n", (int)dma_adc_1_buff[0],(int)dma_adc_1_buff[1],(int)dma_adc_1_buff[2],(int)dma_adc_1_buff[3]);
	
	//go through the values that the DMA got and get the ones that we need (currents and bus voltage)
	for (int i =0; i < ADC_1_NUM_ACTIVE_CHANNELS; i++){
		
		switch((dma_adc_1_buff[i] & AFEC_LCDR_CHNB_Msk)){
			
			case AFEC_LCDR_CHNB(ADC_CURRENT_C_CHANNEL):
				raw_currents[2] = (int) (dma_adc_1_buff[i] & AFEC_LCDR_LDATA_Msk);
				ready_values |= (1<<2);
				break;
			
			case AFEC_LCDR_CHNB(ADC_SUPPL_VOLTAGE_CHANNEL):
				raw_voltage = (int) (dma_adc_1_buff[i] & AFEC_LCDR_LDATA_Msk);
				ready_values |= (1<<3);
				break;
			
			default:
				;
		}
	}
	
	//current conversion is done
	//enable the next conversion if continuous mode enabled
	if(is_dma_adc_1_continuous){
		dma_adc_1_enable_for_one_transaction();
	}
	
	
	
	if(ready_values == ALL_VALUES_READY){
		//means we have collected the data from all ADCs
		
		//we would need new values for next loop
		ready_values = 0;
		
		//launch control loop
		
		printf("Data collected, launching control loop from adc 1 \n");
		printf("%i %i %i %i  \n", raw_voltage, raw_currents[0], raw_currents[1], raw_currents[2], raw_currents[3]);
	}
}


void dma_adc_init(void){
	//initialises the standard values for the DMA controller
	
	
	
	
	//DMA channel 0 for ADC 0
	
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
	
	
	
	
	//DMA channel 1 for ADC 1
	
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
		
	
	
	
	
	//set interrupts
	NVIC_EnableIRQ	(XDMAC_IRQn);
	NVIC_SetPriority(XDMAC_IRQn, IRQ_PRIORITY_DMA_ADC);
	
	NVIC_DisableIRQ		(AFEC0_IRQn);
	NVIC_ClearPendingIRQ(AFEC0_IRQn);
	NVIC_DisableIRQ		(AFEC1_IRQn);
	NVIC_ClearPendingIRQ(AFEC1_IRQn);
	
	
	//DMAs are single transaction by default
	dma_adc_0_disable_continuously();
	dma_adc_1_disable_continuously();
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

void dma_adc_0_enable_continuously(void){
	is_dma_adc_0_continuous = true;
	dma_adc_0_enable_for_one_transaction();
}
void dma_adc_1_enable_continuously(void){
	is_dma_adc_1_continuous = true;
	dma_adc_1_enable_for_one_transaction();
}

void dma_adc_0_disable_continuously(void){
	is_dma_adc_0_continuous = false;
}
void dma_adc_1_disable_continuously(void){
	is_dma_adc_1_continuous = false;
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


//reads data from the DMA buffers
int adc_read(struct adc_async_descriptor *const descr, const uint8_t channel){
	//we might need to worry about race contitions
	//not implemented as of now
	/*
	CRITICAL_SECTION_ENTER()
	
	CRITICAL_SECTION_LEAVE()
	*/
	
	if (descr == (&ADC_0)){
		for (int i =0; i<ADC_0_NUM_ACTIVE_CHANNELS; i++){
			if((dma_adc_0_buff[i] & AFEC_LCDR_CHNB_Msk) == AFEC_LCDR_CHNB(channel)){
				return dma_adc_0_buff[i];
			}
		}
	}
	
	if (descr == (&ADC_1)){
		for (int i =0; i<ADC_1_NUM_ACTIVE_CHANNELS; i++){
			if((dma_adc_1_buff[i] & AFEC_LCDR_CHNB_Msk) == AFEC_LCDR_CHNB(channel)){
				return dma_adc_1_buff[i];
			}
		}
	}
	
	
	return 0;
}