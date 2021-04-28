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
#include "Time_Tester.h"

float fake_angle;
// digital low pass filter stuff
void init_LPF(void){
	DLPF_Init(&DLPF_Curr_A,2,1500,15000);
	DLPF_Init(&DLPF_Curr_B,2,1500,15000);
	DLPF_Init(&DLPF_Curr_C,2,1500,15000);
	
	//do not remove this printf. For some reason it is fixing a linking error where if it isn't here control startup wont work and the program doesnt run. Idk why
	printf("lpf init");
	fake_angle = 0;
}

float gather_data_time, control_dummy_time;

//get position data, timing and torque request
void gather_control_data(void){
	//get torque request
	control_torque_request = 0;
	
	get_Data_Pos(&(control_pos_sens_deltas[0]), &control_pos_sens_sector, &control_pos_sens_time_in_current_sector);
	
	
	//do last as this is the most frequently updated data
	encoder_get_angle(& control_encoder_angle);
	
	gather_data_time = time_get_delta_us();
}

//for testing low pass filter
//float fy_z0, fy_z1, fy_z2, fy_z3, fy_z4;
//float fx_z0, fx_z1, fx_z2, fx_z3, fx_z4;
int fcntr;
int tcntr;


int control_delay;
//process ADC data and start the control loop
//this is being called from the ADC DMA, so we know that the analog sensor values passed are not going to change, hence use pointers instead of copyin data
void start_control_loop_dummy(int * currentsss, int voltageee){
	//control_currents[0] = reconstruct_curr_A(raw_currents[0]);
	
	
	
	//for testing low pass filter
	/*
	fx_z0 = reconstruct_curr_A(raw_currents[0]);
	fy_z0 = 0.023 * fy_z1 + 0.511 * fx_z0 + 0.511 * fx_z1;	//5khz cutoff
	//fy_z0 = -0.522 * fy_z1 + 0.239 * fx_z0 + 0.239 * fx_z1;	
	fcntr++;
	if(fcntr == 15000){
		fcntr =0;
		printf("X = %f \t %f \t %f \t %f \t %f \t \n",fx_z0, fx_z1, fx_z2, fx_z3, fx_z4);
		printf("Y = %f \t %f \t %f \t %f \t %f \t \n",fy_z0, fy_z1, fy_z2, fy_z3, fy_z4);
		DLPF_Print_IO(&DLPF_Curr_A);
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
	
	*/
	
	//store in local variables to prevent data from being overwritten
	control_currents[0] = DLPF_Filter(&DLPF_Curr_A,reconstruct_curr_A( currentsss[0]));
	control_currents[1] = DLPF_Filter(&DLPF_Curr_B,reconstruct_curr_B( currentsss[1]));
	control_currents[2] = DLPF_Filter(&DLPF_Curr_C,reconstruct_curr_C( currentsss[2]));
	
	control_supply_voltage = reconstruct_bus_voltage( voltageee);
	
	//for testing
	control_torque_request = 1.0;
	tcntr++;
	if(tcntr == 45000){
		tcntr = 45000-1;
		control_torque_request = 2.0;
	}
	//control_encoder_angle = 3.1416/(2*5*15);
	fake_angle =  fake_angle + 1 /(5*15 * 15000);
	
	fcntr++;
	if(fcntr == 10000){
		fcntr =0;
		//printf("gather data time %f \t control dummy time %f \n", gather_data_time, control_dummy_time);
		
		//printf("\n startup current %f \t %f",control_currents[0], control_currents[1]);
		
		// !!!!!!!!!!!!!!!!! DO NOT PRINT MULTIPLE OF THESE AT ONCE it breaks the program for some reason
// 		printf("LPF A \n");
// 		DLPF_Print_IO(&DLPF_Curr_A);
// 		printf("LPF B \n");
// 		DLPF_Print_IO(&DLPF_Curr_B);
// 		printf("LPF C \n");
// 		DLPF_Print_IO(&DLPF_Curr_C);
	}
	
	control_delay++;
	if(control_delay >10){
		control_delay = 10;
		//printf("fake %f \n", fake_angle);
		//start control loop below
		Control(control_torque_request, control_supply_voltage, control_pos_sens_sector, control_pos_sens_time_in_current_sector, (float *) control_pos_sens_deltas, control_encoder_angle);
		//controlV(control_torque_request, control_supply_voltage, control_pos_sens_sector, control_pos_sens_time_in_current_sector, (float *) control_pos_sens_deltas, control_encoder_angle);
	}
}