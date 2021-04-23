/*
 * ControlStartup.c
 *
 * Created: 28/02/2021 17:04:29
 *  Author: Ruzhev
 */ 


#include "ControlStartup.h"

#include "Encoder.h"
#include "PositionSensors.h"
#include "User_adc.h"
#include "AnalogSensorConversion.h"
#include "Control.h"




//get position data, timing and torque request
void gather_control_data(void){
	//get torque request
	control_torque_request = 0;
	
	get_Data_Pos(&(control_pos_sens_deltas[0]), &control_pos_sens_sector, &control_pos_sens_time_in_current_sector);
	
	
	//do last as this is the most frequently updated data
	encoder_get_angle(& control_encoder_angle);
}


//process ADC data and start the control loop
//this is being called from the ADC DMA, so we know that the analog sensor values passed are not going to change, hence use pointers instead of copyin data
void start_control_loop_dummy(int * raw_currents, int raw_voltage){
	control_currents[0] = reconstruct_curr_A(raw_currents[0]);
	control_currents[1] = reconstruct_curr_B(raw_currents[1]);
	control_currents[2] = reconstruct_curr_C(raw_currents[2]);
	
	control_supply_voltage = reconstruct_bus_voltage(raw_voltage);
	
	//for testing
	control_torque_request = 2.5;
	
	//start control loop below
	Control(control_torque_request, control_supply_voltage, control_pos_sens_sector, control_pos_sens_time_in_current_sector, &control_pos_sens_deltas, control_encoder_angle);
	//controlV(control_torque_request, control_supply_voltage, control_pos_sens_sector, control_pos_sens_time_in_current_sector, &control_pos_sens_deltas, control_encoder_angle);
}