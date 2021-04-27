/*
 * ControlStartup.h
 *
 * Created: 28/02/2021 17:04:42
 *  Author: Ruzhev
 */ 


#ifndef CONTROLSTARTUP_H_
#define CONTROLSTARTUP_H_

#include "User_Config.h"
#include "Digital_Low_Pass_Filter.h"

//all data values will be local to prevent race conditions in the control loop
int control_pos_sens_sector;
float control_pos_sens_deltas[POS_SENS_DELTAS_SIZE];
float control_pos_sens_time_in_current_sector;

float control_torque_request;

float control_encoder_angle;

float control_currents[3];
float control_supply_voltage;

//digital low pass filters for the current sensors
struct DLPF DLPF_Curr_A;
struct DLPF DLPF_Curr_B;
struct DLPF DLPF_Curr_C;

void init_LPF(void);







//get position data, timing and torque request
void gather_control_data(void);

//process ADC data and start the control loop
//this is being called from the ADC DMA, so we know that the analog sensor values passed are not going to change, hence use pointers instead of copyin data
void start_control_loop_dummy(int * currentsss, int voltageee);


#endif /* CONTROLSTARTUP_H_ */