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
#include "Time_Tester.h"



#include <atmel_start.h>





//enables calling the control loop from the DMA
void enable_control(void){
	is_control_enabled = true;
}
void disable_control(void){
	is_control_enabled = false;
}



//arrays for passing on the values to the control functions
float currents[3];
float voltage;
int currents_int[3];
int voltage_int;

//curr A,B,C are 0,1,2 and voltage is 3
static char ready_values = 0;
#define ALL_VALUES_READY (0b1111)

int adc_cntr;

//callback functions for when transactions are complete
static void dma_adc_0_callback(struct _dma_resource *resource){
	// the DMA writes to system memory. The data has probably been cached which would cause
	// the processor to grab it from the cache and thus not have up to date information
	// This command tells it to not trust this cache and instead fetch from system memory
	SCB_InvalidateDCache_by_Addr((uint32_t *) dma_adc_0_buff, 32);	
	
	
	//let other functions know that a write operation has occurred
	has_0_triggered = true;
	
	//just for testing
	//printf("interrupt - ADC 0 - %i %i %i %i %i %i  \n", (int)dma_adc_0_buff[0],(int)dma_adc_0_buff[1],(int)dma_adc_0_buff[2],(int)dma_adc_0_buff[3], (int)dma_adc_0_buff[4],(int)dma_adc_0_buff[5]);
	
	int raw_data;	//temporary variable
	//go through the values that the DMA got and get the ones that we need (currents and bus voltage)
	for (int i =0; i<ADC_0_NUM_ACTIVE_CHANNELS; i++){
		
		switch((dma_adc_0_buff[i] & AFEC_LCDR_CHNB_Msk)){
			
			case AFEC_LCDR_CHNB(ADC_CURRENT_A_CHANNEL):
				//raw_data = (int) (dma_adc_0_buff[i] & AFEC_LCDR_LDATA_Msk);
				//apply low pass filter
				//currents[0] = DLPF_Filter(&DLPF_Curr_A,reconstruct_curr_A(raw_data));
				//currents[0] = reconstruct_curr_A(raw_data);
				currents_int[0]= (int) (dma_adc_0_buff[i] & AFEC_LCDR_LDATA_Msk);
				ready_values |= (1<<0);
				break;
			
			case AFEC_LCDR_CHNB(ADC_CURRENT_B_CHANNEL):
				raw_data = (int) (dma_adc_0_buff[i] & AFEC_LCDR_LDATA_Msk);
				//apply low pass filter
				//currents[1] = DLPF_Filter(&DLPF_Curr_B,reconstruct_curr_B(raw_data));
				//currents[1] = reconstruct_curr_B(raw_data);
				currents_int[1]= (int) (dma_adc_0_buff[i] & AFEC_LCDR_LDATA_Msk);
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
	
	
	
	//for time diagram testing
	time_delta_adc_0 = time_get_delta_us();
	
	
	
	if(ready_values == ALL_VALUES_READY && is_control_enabled){
		//means we have collected the data from all ADCs
		
		adc_cntr++;
		if(adc_cntr == 12000){
			adc_cntr=0;
			// !!!!! DO NOT PRINT LPF STUFF FROM HERE, causes the program to crash for some reason. Do it from control startup
			//DLPF_Print_Coeff(&DLPF_Curr_A);
			//printf("Data collected, launching control loop from adc 0\n");
			//printf("%f %f %f %f  \n", voltage, currents[0], currents[1], currents[2]);
		}
		
		//we would need new values for next loop
		ready_values = 0;
		
		//launch control loop
		start_control_loop_dummy((int *) &currents_int, voltage_int);
		

	}
	
}

static void dma_adc_1_callback(struct _dma_resource *resource){
	// the DMA writes to system memory. The data has probably been cached which would cause
	// the processor to grab it from the cache and thus not have up to date information
	// This command tells it to not trust this cache and instead fetch from system memory
	SCB_InvalidateDCache_by_Addr((uint32_t *) dma_adc_1_buff, 32);
	
	
	//let other functions know that a write operation has occurred
	has_1_triggered = true;
	
	//just for testing
	//printf("interrupt - ADC 1 - %i %i %i %i  \n", (int)dma_adc_1_buff[0],(int)dma_adc_1_buff[1],(int)dma_adc_1_buff[2],(int)dma_adc_1_buff[3]);
	
	int raw_data;	//temporary variable
	//go through the values that the DMA got and get the ones that we need (currents and bus voltage)
	for (int i =0; i < ADC_1_NUM_ACTIVE_CHANNELS; i++){
		
		switch((dma_adc_1_buff[i] & AFEC_LCDR_CHNB_Msk)){
			
			case AFEC_LCDR_CHNB(ADC_CURRENT_C_CHANNEL):
				//raw_data = (int) (dma_adc_1_buff[i] & AFEC_LCDR_LDATA_Msk);
				//apply low pass filter
				//currents[2] = DLPF_Filter(&DLPF_Curr_C,reconstruct_curr_C(raw_data));
				//currents[2] = reconstruct_curr_C(raw_data);
				currents_int[2]= (int) (dma_adc_1_buff[i] & AFEC_LCDR_LDATA_Msk);
				ready_values |= (1<<2);
				break;
			
			case AFEC_LCDR_CHNB(ADC_SUPPL_VOLTAGE_CHANNEL):
				//raw_data = (int) (dma_adc_1_buff[i] & AFEC_LCDR_LDATA_Msk);
				//might need low pass filerting in future 
				//voltage = reconstruct_bus_voltage(raw_data);
				//voltage_int= (int) (dma_adc_1_buff[i] & AFEC_LCDR_LDATA_Msk);
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
	
	//for time diagram testing
	time_delta_adc_1 = time_get_delta_us();
	
	if(ready_values == ALL_VALUES_READY && is_control_enabled){
		//means we have collected the data from all ADCs
		
		adc_cntr++;
		if(adc_cntr == 12000){
			adc_cntr=0;
			// !!!!! DO NOT PRINT LPF STUFF FROM HERE, causes the program to crash for some reason. Do it from control startup
			//DLPF_Print_Coeff(&DLPF_Curr_A);
			//printf("Data collected, launching control loop from adc 1 \n");
			//printf("%f %f %f %f  \n", voltage, currents[0], currents[1], currents[2]);
		}
		
		//we would need new values for next loop
		ready_values = 0;
		
		//launch control loop
		//start_control_loop_dummy((float *) &currents, voltage);
		start_control_loop_dummy((int *) &currents_int, voltage_int);
		
		
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
	
	
	has_0_triggered = false;
	has_1_triggered = false;
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
	//null checking variable to see if a write has occured
	has_0_triggered = false;
	has_1_triggered = false;
	
		
	if (descr == (&ADC_0)){
		for (int i =0; i<ADC_0_NUM_ACTIVE_CHANNELS; i++){
			if((dma_adc_0_buff[i] & AFEC_LCDR_CHNB_Msk) == AFEC_LCDR_CHNB(channel)){
				int temp = (int) (dma_adc_0_buff[i] & AFEC_LCDR_LDATA_Msk);
				
				
				if(has_0_triggered){
					//data was altered while we were reading
					//retake data
					return adc_read(descr, channel);
				}
				
				return temp;
			}
		}
	}
	
	if (descr == (&ADC_1)){
		for (int i =0; i<ADC_1_NUM_ACTIVE_CHANNELS; i++){
			if((dma_adc_1_buff[i] & AFEC_LCDR_CHNB_Msk) == AFEC_LCDR_CHNB(channel)){
				int temp = (int) (dma_adc_1_buff[i] & AFEC_LCDR_LDATA_Msk);
				
				if(has_1_triggered){
					//data was altered while we were reading
					//retake data
					return adc_read(descr, channel);
				}
				
				return temp;
			}
		}
	}
	
	
	return 0;
}