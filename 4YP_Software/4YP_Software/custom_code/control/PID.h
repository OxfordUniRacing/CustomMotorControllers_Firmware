/*
 * PID.h
 *
 * Created: 18/01/2021 14:42:16
 *  Author: Ben
 */ 


#ifndef PID_H_
#define PID_H_

#include <atmel_start.h>

struct PID_instance{
	float Kp, Ki, Kd;
	float previous_error;
	float integral_v;
	float time_step;
	bool is_using_constant_timestep;
};
	
void PID_init_cts(struct PID_instance* PID,float Kp,float Ki,float Kd, float time_step);
float runPID(struct PID_instance* PID, float ref, float feedback);


#endif /* PID_H_ */