/*
 * Control.c
 *
 * Created: 15/01/2021 11:48:31
 *  Author: Ben
 */ 

#include "Control.h"
#include "ControlConfig.h"
#include "ControlStartup.h"
#include "EstimateTheta.h"
#include "Time_Tester.h"
#include "User_pwm.h"


#include <atmel_start.h>



int cntrrr = 0;
float control_time;

void Init_Control(void) {
	arm_mat_init_f32 (&A,A_rows,A_cols,(float32_t *)A_data);    //MATRIX EXAMPLE
	arm_mat_init_f32 (&I,I_rows,I_cols,(float32_t *)control_currents);    //create current vector
	arm_mat_init_f32 (&PWM,PWM_rows,PWM_cols,(float32_t *)PWM_data);    //create pwm vector
	
	PID_init_cts(&PID_d, PID_d_Kp, PID_d_Ki, PID_d_Kd,(float)1.0/15000.0);		//initialise the PID controller for d and q values
	PID_init_cts(&PID_q, PID_q_Kp, PID_q_Ki, PID_q_Kd,(float)1.0/15000.0);
	
	oldtorquerequest = 0;
	
		//do not remove this printf. For some reason it is fixing a linking error where if it isn't here control startup wont work and the program doesnt run. Idk why

	printf("\n");
}


void getIqId_r(float torquerequest, float* Iq_r, float* Id_r, float V_dc) {		//Calculates reference currents based on the torque requests 
	
	//float omega_base_e = V_dc*LST_SQ_OMEGA_BASE_E				//FIELD WEAKENING PART TO FINISH
	//if(omega_e > omega_base_e ){}
		
	float I_m = 2* torquerequest / (3*PP*FLUX_PM);
	
	if (I_m > I_MAX){I_m = I_MAX;}
	*Id_r = C1 - sqrt(C1_SQR - 0.5*(I_m*I_m));
	
	*Iq_r = sqrt(I_m*I_m - (*Id_r)*(*Id_r));
}	
int cntrrar;
void SVPWM(float Va_aim, float Vb_aim, float* PWM, float V_dc) {							//Space Vector Modulation Function
	float Vc_aim;
	Vc_aim = -Vb_aim - Va_aim;										//Calculates third voltage aim
	
	float Va_comp, Vb_comp, Vc_comp;
	Va_comp = (V_dc-Va_aim)/V_dc;									//normalise 
	Vb_comp = (V_dc-Vb_aim)/V_dc;	
	Vc_comp = (V_dc-Vc_aim)/V_dc;
	
	float V_min;
	
	if((Va_comp<Vb_comp)&&(Va_comp<Vc_comp)) {					//Finds minimum 
		V_min = Va_comp;
	}else{
		if(Vb_comp<Vc_comp){
			V_min = Vb_comp;
		}else{
			V_min = Vc_comp;
		}
	}
	
	float Va_dc, Vb_dc, Vc_dc;					//does down clamping and sets minimum to zero, subtracting minimum from all three
	PWM[0] = 1 - (Va_comp - V_min);
	PWM[1] = 1 - (Vb_comp - V_min);
	PWM[2] = 1 - (Vc_comp - V_min);
	cntrrar++;
	if(cntrrar == 15000){
		cntrrar = 0;
		
		//printf("\n PWM A = %f \t PWM B = %f \t PWM C - %f ", PWM[0], PWM[1], PWM[2]);
	}

}

float ffake_angle;
void Control(float torquerequest, float V_dc, int pos_HS_state, float pos_HS_t1, float *pos_HS_dts, float pos_ENC_angle) {
	ffake_angle =  ffake_angle + 20.00 /(15000.0);
	
	V_dc = 20;
	
	//Limit torque request rate
	if (torquerequest - T_RATE_UP > oldtorquerequest){torquerequest = oldtorquerequest + T_RATE_UP;} //Limit Increase Rate
	if (torquerequest + T_RATE_DOWN < oldtorquerequest){torquerequest = oldtorquerequest - T_RATE_DOWN;} //Limit Decrease Rate
	oldtorquerequest = torquerequest;	//Update the new old value
	
	
	
	//time_record_timestamp();
	float Iq_r, Id_r;
	getIqId_r(torquerequest, &Iq_r, &Id_r, V_dc);	//Get the id and iq requested current
		//Iq_r = 10;
		//Id_r = 0;
	
	
	theta_e = EstimateTheta(pos_HS_state, pos_HS_t1, &pos_HS_dts, pos_ENC_angle);
	//theta_e = ffake_angle;
	//theta_e = 0;
	float sintheta_e = sin(theta_e);
	float costheta_e = cos(theta_e);	//(Currently uses fast sin and cosine)
	
	float I_alpha, I_beta;
	arm_clarke_f32(control_currents[0],control_currents[1],&I_alpha,&I_beta); //Does clarke transform
	float I_d, I_q; 
	arm_park_f32(I_alpha,I_beta, &I_d, &I_q, sintheta_e, costheta_e);	//Does Park transform 
	
	//float Vd_aim, Vq_aim;		//defined somewhre else
	Vd_aim = runPID(&PID_d, Id_r, I_d);								//PID 
	Vq_aim = runPID(&PID_q, Iq_r, I_q);

	float Valpha_aim, Vbeta_aim;
	arm_inv_park_f32(Vd_aim,Vq_aim,&Valpha_aim,&Vbeta_aim,sintheta_e,costheta_e);	//Inverse Park transform
	
	float Va_aim, Vb_aim;
	arm_inv_clarke_f32(Valpha_aim,Vbeta_aim,&Va_aim,&Vb_aim);		//Inverse clarke transform
	
	SVPWM(Va_aim, Vb_aim, (float32_t *)PWM_data, V_dc);										//Updates PWM values using space vector PWM
	
	update_PWM((float32_t *)PWM_data);
	
	control_time = time_get_delta_us();
	
	cntrrr++;
	if(cntrrr == 1){
		cntrrr = -15000;
// 		printf("\n");
// 		printf("\nTime Control = %f us ",control_time);
// 		printf("\nTorque request - %f ",torquerequest);
// 
// 		printf("\n Va_aim = %f \t Vb_aim = %f \t theta = %f; \t sintheta = %f", Va_aim, Vb_aim, theta_e, sintheta_e);
// 		printf("\n A amps = %f \t B amps = %f", control_currents[0],control_currents[1]);
//  		printf("\n I_d = %f \t I_q = %f", I_d, I_q);
//  		printf("\n Vd_aim = %f \t Vq_aim = %f", Vd_aim, Vq_aim);
//  		printf("\n Id_r = %f \t Iq_r = %f", Id_r, Iq_r);
// 		printf("\n Ia = %f \t Ib = %f", I_alpha, I_beta);
	}
}





void controlV(float torquerequest, float V_dc, int pos_HS_state, float pos_HS_t1, float *pos_HS_dts, float pos_ENC_angle) {
	
	V_dc = 8;
	//Limit torque request rate
	//if (torquerequest - T_RATE_UP > oldtorquerequest){torquerequest = oldtorquerequest + T_RATE_UP;} //Limit Increase Rate
	//if (torquerequest + T_RATE_DOWN < oldtorquerequest){torquerequest = oldtorquerequest - T_RATE_DOWN;} //Limit Decrease Rate
	//oldtorquerequest = torquerequest;	//Update the new old value
	
	
	//float Iq_r, Id_r;
	//getIqId_r(torquerequest, &Iq_r, &Id_r, V_dc);	//Get the id and iq requested current
	

	
	theta_e = EstimateTheta(pos_HS_state, pos_HS_t1, &pos_HS_dts, pos_ENC_angle);
	float sintheta_e = sin(theta_e);
	float costheta_e = cos(theta_e);	//(Currently uses fast sin and cosine)
	
	//float I_alpha, I_beta;
	//arm_clarke_f32(control_currents[0],control_currents[1],&I_alpha,&I_beta); //Does clarke transform
	//float I_d, I_q;
	//arm_park_f32(I_alpha,I_beta, &I_d, &I_q, sintheta_e, costheta_e);	//Does Park transform
	
	
	
	
	float Valpha_aim, Vbeta_aim;
	arm_inv_park_f32(Vd_aim,Vq_aim,&Valpha_aim,&Vbeta_aim,sintheta_e,costheta_e);	//Inverse Park transform
	
	float Va_aim, Vb_aim;
	arm_inv_clarke_f32(Valpha_aim,Vbeta_aim,&Va_aim,&Vb_aim);		//Inverse clarke transform
	
	SVPWM(Va_aim, Vb_aim, (float32_t *)PWM_data, V_dc);										//Updates PWM values using space vector PWM
	
	update_PWM((float32_t *)PWM_data);
	
	
	cntrrr++;
	if(cntrrr == 15000){
		cntrrr = 0;
		
		//printf("\n Va_aim = %f \t Vb_aim = %f \t theta - %f; \t sintheta %f", Va_aim, Vb_aim, theta_e, sintheta_e);
	}
}



void update_PWM(float* PWM){
	pwm_set_duty(PWM_PHASE_A, (int) ((PWM_PERIOD-1) * PWM[0]));
	pwm_set_duty(PWM_PHASE_B, (int) ((PWM_PERIOD-1) * PWM[1]));
	pwm_set_duty(PWM_PHASE_C, (int) ((PWM_PERIOD-1) * PWM[2]));
	
}