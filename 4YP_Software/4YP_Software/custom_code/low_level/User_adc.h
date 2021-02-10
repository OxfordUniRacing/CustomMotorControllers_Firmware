/*
 * User_adc.h
 *
 * Created: 11/01/2021 17:22:53
 *  Author: Ruzhev
 */ 


#ifndef USER_ADC_H_
#define USER_ADC_H_

#include <hal_adc_sync.h>



//enables/disables all AFECs
void adc_enable_all(void);
void adc_disable_all(void);

//reads ADC value
uint16_t adc_read(struct adc_sync_descriptor *const descr, const uint8_t channel);





#endif /* USER_ADC_H_ */