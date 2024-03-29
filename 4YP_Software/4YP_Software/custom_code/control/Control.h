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
#include "EstimateTheta.h"
#include "PID.h"

// MATRIX EXAMPLE

#define A_cols 2
#define A_rows 3
float A_data [A_cols*A_rows];
arm_matrix_instance_f32 A;


// Current Vector
#define I_cols 1
#define I_rows 3
//float I_data [I_cols*I_rows];			//Using external data from control startup
arm_matrix_instance_f32 I;

// Electric Theta
float theta_e;
float omega_e;

// PWM Duty Cycle output
#define PWM_cols 1
#define PWM_rows 3
float PWM_data [PWM_cols*PWM_rows];
arm_matrix_instance_f32 PWM;

#define PID_q_Kp 0.0104//0.6635081 //0.0415//0.6635081		//From Auto-Tuner 21/04/21
#define PID_q_Ki 8.4348//162.4788 //33.8//162.4788
#define PID_q_Kd 0
struct PID_instance PID_q;

#define PID_d_Kp 0.0078//0.6040525//0.03//0.6040525
#define PID_d_Ki 8.4378//154.1546//32.5//154.1546
#define PID_d_Kd 0
struct PID_instance PID_d;



void Control(float torquerequest, float V_dc, int pos_HS_state, float pos_HS_t1, float *pos_HS_dts, float pos_ENC_angle);
void Init_Control(void);
void controlV(float torquerequest, float V_dc, int pos_HS_state, float pos_HS_t1, float *pos_HS_dts, float pos_ENC_angle);

void update_PWM(float* PWM);
float Vd_aim, Vq_aim;


#endif /* CONTROL_H_ */