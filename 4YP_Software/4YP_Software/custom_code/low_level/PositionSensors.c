/*
 * PositionSensors.c
 *
 * Created: 25/02/2021 11:29:21
 *  Author: Ruzhev
 */ 


#include "PositionSensors.h"
#include "User_Config.h"
#include "Encoder.h"
//atmel start gives access to the SysTick
#include <atmel_start.h>


int cnt = 0;

static inline void Position_General_Interrupt(void){
	//records the delta value in systick and uses that to calculate delta t from the last position sensor interrupt
	
	//make other functions aware that a write operation has occured
	has_triggered = true;
	
	//get systick value immediately for most accurate result
	int current_systick = SysTick->VAL;
	int delta = pos_sens_last_SysTick_count - current_systick;	//systick decrements
	
	
	
	//printf("%i \n", current_systick);
	//if there was an overflow, account for it
	//NB this can only handle one overflow at max <=> if systick frequency < interrupt frequency timing will be very inaccurate
	if(delta <= 0) delta += (1<<24);	//systick is 24 bit counter
		
		
		
		
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
	//printf("POS 1\n");
}

static void Position_2_Interrupt (void){
	Position_General_Interrupt();
	//printf("POS 2\n");
}
int cntr = 0;
static void Position_3_Interrupt (void){
	Position_General_Interrupt();
	//printf("POS 3\n");
	
	
	// For calibrating the encoder offset
	/*
	if(gpio_get_pin_level(PIN_GPIO_POS_3) == 0){	//0 degrees state is at 1,0,0 ; with pos sens 3 transitioning from 1 to 0
		cntr ++;
		if(cntr ==15*5){
			cntr=0;
			float angleee = 0;
			encoder_get_angle(&angleee);
		
			printf("%f\n",angleee);
		}
	}
	*/
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
	
	/*
	From motor datasheet
						Step1	Step2	Step3	Step4	Step5	Step6	Error1	Error2
	Phase A(U)		->	+		Z		-		-		Z		+		Z		Z
	Phase B(V)		->	Z		+		+		Z		-		-		Z		Z
	Phase C(W)		->	-		-		Z		+		+		Z		Z		Z
	Sensor A (POS1)	->	1		1		0		0		0		1		0		1
	Sensor B (POS2)	->	0		1		1		1		0		0		0		1
	Sensor C (POS3)	->	0		0		0		1		1		1		0		1

	Position convention:
	Step 1 <=> first 60 degree sector
	Step 2 <=> second 60 degree sector etc.

	this means that the moment we transition to state (POS1=1, POS2=0 POS3=0) we are at 0 electrical degrees
	*/

	// sector_lookup_table[POS3][POS2][POS1];
	sector_lookup_table[0][0][1] = 1;
	sector_lookup_table[0][1][1] = 2;
	sector_lookup_table[0][1][0] = 3;
	sector_lookup_table[1][1][0] = 4;
	sector_lookup_table[1][0][0] = 5;
	sector_lookup_table[1][0][1] = 6;
	//Error states
	sector_lookup_table[0][0][0] = -1;
	sector_lookup_table[1][1][1] = -1;
	
	has_triggered = false;
}

//returns time spent in previous sectors
// which sector we are at currently (see above for position convention)
// how much time has elapsed since we entered this sector
void get_Data_Pos (float * previous_deltas, int * current_sector, float * time_in_current_sector){
	//null checking variable to see if a write has occurred
	has_triggered = false;
	
	
	//get systick value immediately for most accurate result
	int current_systick = SysTick->VAL;
	int delta = pos_sens_last_SysTick_count - current_systick;	//systick decrements
	
	//if there was an overflow, account for it
	//NB this can only handle one overflow at max <=> if systick frequency < interrupt frequency timing will be very inaccurate
	if(delta <= 0) delta += (1<<24);	//systick is 24 bit counter
	
	//systick clock frequency equals MCU clock at 300MHz
	(*time_in_current_sector) = (float) delta / 300;
	
	
	//copy data over
	for (int i = 0 ; i < POS_SENS_DELTAS_SIZE; i++){
		previous_deltas[i] = pos_sens_deltas[i];
	}
	
	//get which sector we are in
	(*current_sector) = sector_lookup_table	[gpio_get_pin_level(PIN_GPIO_POS_3)]\
											[gpio_get_pin_level(PIN_GPIO_POS_2)]\
											[gpio_get_pin_level(PIN_GPIO_POS_1)];
											
											
	if(has_triggered){
		// this means the value of has_triggered was modified while we were reading the data <=> retake reading
		// since data writes are rare there is no chance of a long wait on the recursive function
		get_Data_Pos (previous_deltas, current_sector, time_in_current_sector);
	}

}