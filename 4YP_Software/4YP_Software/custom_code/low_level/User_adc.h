/*
 * User_adc.h
 *
 * Created: 11/01/2021 17:22:53
 *  Author: Ruzhev
 */ 


#ifndef USER_ADC_H_
#define USER_ADC_H_

#include <hal_adc_async.h>

void dma_adc_init(void);
void dma_adc_0_enable_for_one_transaction(void);
void dma_adc_1_enable_for_one_transaction(void);

//enables/disables all AFECs
void adc_enable_all(void);
void adc_disable_all(void);






#endif /* USER_ADC_H_ */