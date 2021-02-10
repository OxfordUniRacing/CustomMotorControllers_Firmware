/*
 * PID.c
 *
 * Created: 18/01/2021 14:42:08
 *  Author: Ben
 */ 

#include "PID.h"

void PID_init_ncts(struct PID_instance* PID,float Kp,float Ki,float Kd){
	PID->Kd = Kd;
	PID->Ki = Ki;
	PID->Kp = Kp;							//sets gains
	PID->integral_v = 0;
	PID->previous_error = 0;
	PID->is_using_constant_timestep = false;
}


void PID_init_cts(struct PID_instance* PID,float Kp,float Ki,float Kd, float time_step){
	PID->Kd = Kd;
	PID->Ki = Ki;
	PID->Kp = Kp;							//sets gains
	PID->integral_v = 0;
	PID->previous_error = 0;
	PID->is_using_constant_timestep = true;
	PID->time_step = time_step;
}

float runPID(struct PID_instance* PID, float ref, float feedback){
	if(!PID->is_using_constant_timestep){
		//PID->time_step = getTimeStep();
	}
	
	float Ap, Ad, Ai;
	float error = ref - feedback;
	
	Ap = error*PID->Kp;
	Ad = PID->Kd*(error - PID->previous_error)/PID->time_step;
	Ai = PID->Ki*(error*PID->time_step) + PID->integral_v;
	
	if( ((Ai>=Ap) & (Ap>=0)) | ((Ai<=Ap) & (Ap<=0)) )   Ai = Ap;		//set anti-windup for integral action
	
	PID->previous_error = error;
	return (Ap + Ai + Ad);

}