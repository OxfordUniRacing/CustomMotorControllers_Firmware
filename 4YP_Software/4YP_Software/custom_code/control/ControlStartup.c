/*
 * ControlStartup.c
 *
 * Created: 28/02/2021 17:04:29
 *  Author: Ruzhev
 */ 


#include "Encoder.h"
#include "PositionSensors.h"
#include "User_adc.h"




//get position data, timing and torque request
void gather_control_data(void){
	//get torque request
	control_torque_request = 0;
	
	get_Data_Pos(&control_pos_sens_deltas, &control_pos_sens_sector, &control_pos_sens_time_in_current_sector);
	
	
	//do last as this is the most frequently updated data
	control_encoder_angle = encoder_get_angle();
}


//process ADC data and start the control loop
//this is being called from the ADC DMA, so we know that the analog sensor values passed are not going to change, hence use pointers instead of copyin data
void start_control_loop_dummy(int * raw_currents, int raw_voltage){
	
	
	
	
	//start control loop below
	
}