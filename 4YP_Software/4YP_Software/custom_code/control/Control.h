/*
 * Control.h
 *
 * Created: 15/01/2021 11:48:15
 *  Author: Ben
 */ 


#ifndef CONTROL_H_
#define CONTROL_H_



//add these 3 lines before every instance of arm_math.h ; 
//configuration macro to enable the correct code within the library
#ifndef ARM_MATH_CM7
#define ARM_MATH_CM7
#endif
//always have atmel_start.h before arm_math.h -> seems to be fixing some issiues
#include <atmel_start.h>
#include "arm_math.h"
#include "getTheta.h"
#include "PID.h"

// MATRIX EXAMPLE

#define A_cols 2
#define A_rows 3
float A_data [A_cols*A_rows];
arm_matrix_instance_f32 A;


// Current Vector
#define I_cols 1
#define I_rows 3
float I_data [I_cols*I_rows];
arm_matrix_instance_f32 I;

// Electric Theta
float theta_e;
float omega_e;

// PWM Duty Cycle output
#define PWM_cols 1
#define PWM_rows 3
float PWM_data [PWM_cols*PWM_rows];
arm_matrix_instance_f32 PWM;

#define PID_q_Kp 21.22		//set these
#define PID_q_Ki 21.22
#define PID_q_Kd 0
struct PID_instance PID_q;

#define PID_d_Kp 21.22
#define PID_d_Ki 21.22
#define PID_d_Kd 0
struct PID_instance PID_d;


void Control(float torquerequest, float V_dc);




#endif /* CONTROL_H_ */