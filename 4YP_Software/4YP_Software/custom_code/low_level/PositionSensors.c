/*
 * PositionSensors.c
 *
 * Created: 25/02/2021 11:29:21
 *  Author: Ruzhev
 */ 


#include "PositionSensors.h"
#include "User_Config.h"
//atmel start gives access to the SysTick
#include <atmel_start.h>


static inline void Position_General_Interrupt(void){
	//records the delta value in systick and uses that to calculate delta t from the last position sensor interrupt
	
	int current_systick = SysTick->VAL;
	int delta = current_systick - pos_sens_last_SysTick_count;
		
	//if there was an overflow, account for it
	//NB this can only handle one overflow at max <=> if systick frequency < interrupt frequency timing will be very inaccurate
	if(delta <= 0) delta += (1<<25);	//systick is 24 bit counter
		
		
		
	//shuffle the data along to make space for the new delta that we are generating
	for (int i = POS_SENS_DELTAS_SIZE-1 ; i > 0; i--){
		pos_sens_deltas[i] = pos_sens_deltas[i-1];
	}
		
	//systick clock frequency equals MCU clock at 300MHz
	pos_sens_deltas [0] = (float) delta / 300;
	
	// update previous systick value
	pos_sens_last_SysTick_count = current_systick;
}

static void Position_1_Interrupt (void){
	Position_General_Interrupt();
	printf("POS 1\n");
}

static void Position_2_Interrupt (void){
	Position_General_Interrupt();
	printf("POS 2\n");
}

static void Position_3_Interrupt (void){
	Position_General_Interrupt();
	printf("POS 3\n");
}

void pos_sens_init (void){
	//set interrupt handlers
	ext_irq_register(PIO_PD19_IDX, Position_3_Interrupt);		//POS 3
	ext_irq_register(PIO_PA2_IDX, Position_2_Interrupt);		//POS 2
	ext_irq_register(PIO_PA5_IDX, Position_1_Interrupt);		//POS 1
	
	NVIC_EnableIRQ	(PIOA_IRQn);
	NVIC_SetPriority(PIOA_IRQn, IRQ_PRIORITY_POSITION_SENS);
	
	NVIC_EnableIRQ	(PIOD_IRQn);
	NVIC_SetPriority(PIOD_IRQn, IRQ_PRIORITY_POSITION_SENS);

	
	//SysTick starting from 0
	pos_sens_last_SysTick_count = 0;
	SysTick->VAL = 0;
	//make the load value max, so that the overflows are as rare as possible
	SysTick->LOAD = (0xFFFFFF);
}

//returns how many time steps are recorded in the times array
int pos_sens_get_deltas_us (float * deltas){
	//return pointer to the local times array
	deltas = &(pos_sens_deltas[0]);
	
	return POS_SENS_DELTAS_SIZE;
}