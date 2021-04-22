/*
 * User_adc.h
 *
 * Created: 11/01/2021 17:22:53
 *  Author: Ruzhev
 */ 


#ifndef USER_ADC_H_
#define USER_ADC_H_

#include "User_Config.h"



//buffers for the DMA to put the output of the ADCs
static uint32_t dma_adc_0_buff[ADC_0_NUM_ACTIVE_CHANNELS];
static uint32_t dma_adc_1_buff[ADC_1_NUM_ACTIVE_CHANNELS];

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