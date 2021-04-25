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

float fy_z0, fy_z1, fy_z2, fy_z3, fy_z4;
float fx_z0, fx_z1, fx_z2, fx_z3, fx_z4;
int fcntr;
//process ADC data and start the control loop
//this is being called from the ADC DMA, so we know that the analog sensor values passed are not going to change, hence use pointers instead of copyin data
void start_control_loop_dummy(int * raw_currents, int raw_voltage){
	//control_currents[0] = reconstruct_curr_A(raw_currents[0]);
	
	fx_z0 = reconstruct_curr_A(raw_currents[0]);
	//fy_z0 = 0.023 * fy_z1 + 0.511 * fx_z0 + 0.511 * fx_z1;	//5khz cutoff
	fy_z0 = -0.522 * fy_z1 + 0.239 * fx_z0 + 0.239 * fx_z1;	//3khz cutoff
	fcntr++;
	if(fcntr == 15000){
		fcntr =0;
		printf("X = %f \t %f \t %f \t %f \t %f \t \n",fx_z0, fx_z1, fx_z2, fx_z3, fx_z4);
		printf("Y = %f \t %f \t %f \t %f \t %f \t \n",fy_z0, fy_z1, fy_z2, fy_z3, fy_z4);
	}
	
	fx_z4 = fx_z3;
	fx_z3 = fx_z2;
	fx_z2 = fx_z1;
	fx_z1 = fx_z0;
	
	fy_z4 = fy_z3;
	fy_z3 = fy_z2;
	fy_z2 = fy_z1;
	fy_z1 = fy_z0;
	control_currents[0] = fy_z0;
	
	
	
	
	control_currents[1] = reconstruct_curr_B(raw_currents[1]);
	control_currents[2] = reconstruct_curr_C(raw_currents[2]);
	
	control_supply_voltage = reconstruct_bus_voltage(raw_voltage);
	
	//for testing
	control_torque_request = 2.5;
	
	//start control loop below
	//Control(control_torque_request, control_supply_voltage, control_pos_sens_sector, control_pos_sens_time_in_current_sector, &control_pos_sens_deltas, control_encoder_angle);
	//controlV(control_torque_request, control_supply_voltage, control_pos_sens_sector, control_pos_sens_time_in_current_sector, &control_pos_sens_deltas, control_encoder_angle);
}