/*
 * User_adc.h
 *
 * Created: 11/01/2021 17:22:53
 *  Author: Ruzhev
 */ 


#ifndef USER_ADC_H_
#define USER_ADC_H_

#include "User_Config.h"




//arrays for passing on the values to the control functions
int currents_int[3];
int voltage_int;

//for time diagram testing
float time_delta_adc_0, time_delta_adc_1;
int time_print_counter;

//buffers for the DMA to put the output of the ADCs
// for cache reasons (see the explanation in the dma callback function) the array needs 
// to be aligned to the cache lines (32-byte) and have a size that is multiple of the cache size (aka 32 bytes)
__attribute__ ((aligned (32))) static uint32_t dma_adc_0_buff[16];
__attribute__ ((aligned (32))) static uint32_t dma_adc_1_buff[16];

//variables for whether to enable continuous DMA or single transactions
bool is_dma_adc_0_continuous;
bool is_dma_adc_1_continuous;

//use this variables to check if values have been altered during a read operation
bool has_0_triggered;
bool has_1_triggered;

bool is_control_enabled;
void enable_control(void);
void disable_control(void);


void dma_adc_init(void);

void dma_adc_0_enable_for_one_transaction(void);
void dma_adc_1_enable_for_one_transaction(void);

void dma_adc_0_enable_continuously(void);
void dma_adc_1_enable_continuously(void);
void dma_adc_0_disable_continuously(void);
void dma_adc_1_disable_continuously(void);

//enables/disables all AFECs
void adc_enable_all(void);
void adc_disable_all(void);


//reads data from the DMA buffers
int adc_read(struct adc_async_descriptor *const descr, const uint8_t channel);






#endif /* USER_ADC_H_ */