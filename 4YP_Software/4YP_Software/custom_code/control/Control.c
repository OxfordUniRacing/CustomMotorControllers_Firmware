/*
 * Control.c
 *
 * Created: 15/01/2021 11:48:31
 *  Author: Ben
 */ 

#include "Control.h"
#include "ControlConfig.h"

void Init_Control(void){
	arm_mat_init_f32 (&A,A_rows,A_cols,(float32_t *)A_data);    //MATRIX EXAMPLE
	arm_mat_init_f32 (&I,I_rows,I_cols,(float32_t *)I_data);    //create current vector
	arm_mat_init_f32 (&PWM,PWM_rows,PWM_cols,(float32_t *)PWM_data);    //create pwm vector
	
	PID_init_ncts(&PID_d, PID_d_Kp, PID_d_Ki, PID_d_Kd);		//initialise the PID controller for d and q values
	PID_init_ncts(&PID_q, PID_q_Kp, PID_q_Ki, PID_q_Kd);
}


void getIqId_r(float torquerequest, float* Iq_r, float* Id_r, float* V_dc){		//Calculates reference currents based on the torque requests 
	
	//float omega_base_e = V_dc*LST_SQ_OMEGA_BASE_E				//FIELD WEAKENING PART TO FINISH
	//if(omega_e > omega_base_e ){}
		
	float I_m = 2*torquerequest/(3*PP*FLUX_PM);
		if (I_m > I_MAX){I_m = I_MAX;}
	*Id_r = C1 - sqrt(C1_SQR - 0.5*(I_m*I_m));
	*Iq_r = sqrt(I_m*I_m - (*Id_r)*(*Id_r));
	}	

void SVPWM(float Va_aim, float Vb_aim, float* PWM){							//Space Vector Modulation Function
	float Vc_aim;
	Vc_aim = -Vb_aim - Va_aim;										//Calculates third voltage aim
	
	float Va_comp, Vb_comp, Vc_comp;
	Va_comp = (V_dc-Va_aim)/V_dc;									//normalise 
	Vb_comp = (V_dc-Vb_aim)/V_dc;	
	Vc_comp = (V_dc-Vc_aim)/V_dc;
	
	float V_min;
	
	if((Va_comp<Vb_comp)&&(Va_comp<Vc_comp)) {					//Finds minimum 
		V_min = Va_comp;
		}
	else{
		if(Vb_comp<Vc_comp){
		V_min = Vb_comp;
		}
		else{
		V_min = Vc_comp;
		}
	}
	
	float Va_dc, Vb_dc, Vc_dc;					//does down clamping and sets minimum to zero, subtracting minimum from all three
	PWM[0] = Va_comp - V_min;
	PWM[1] = Vb_comp - V_min;
	PWM[2] = Vc_comp - V_min;
	
}

void Control(float torquerequest){
	
	float Iq_r, Id_r;
	getIqId_r(torquerequest, &Iq_r, &Id_r, &V_dc);	//Get the id and iq requested current
	
	theta_e = getTheta();
	float sintheta_e = sin(theta_e);
	float costheta_e = cos(theta_e);	//(Currently uses fast sin and cosine)
	
	float I_alpha, I_beta;
	arm_clarke_f32(I_data[0],I_data[1],&I_alpha,&I_beta); //Does clarke transform
	float I_d, I_q; 
	arm_park_f32(I_alpha,I_beta, &I_d, &I_q, sintheta_e, costheta_e);	//Does Park transform 
	
	float Vd_aim, Vq_aim;
	Vd_aim = runPID(&PID_d, Id_r, I_d);								//PID 
	Vq_aim = runPID(&PID_q, Iq_r, I_q);
	
	float Valpha_aim, Vbeta_aim;
	arm_inv_park_f32(Vd_aim,Vq_aim,&Valpha_aim,&Vbeta_aim,sintheta_e,costheta_e);	//Inverse Park transform
	
	float Va_aim, Vb_aim;
	arm_inv_clarke_f32(Valpha_aim,Vbeta_aim,&Va_aim,&Vb_aim);		//Inverse clarke transform
	
	SVPWM(Va_aim, Vb_aim, (float32_t *)PWM_data);										//Updates PWM values using space vector PWM
	
}

