/*
 * EstimateTheta.c
 *
 * Created: 18/01/2021 14:08:16
 *  Author: Ben
 */ 

#include "EstimateTheta.h"
#include "ControlConfig.h"

float EstimateTheta(int pos_HS_state, float pos_HS_t1, float *pos_HS_dts, float pos_ENC_angle){
	//Estimate angle from combination of maybe encoder and maybe observer and maybe hall sensors
	theta_e_HS = EstimateThetaHS(pos_HS_state, pos_HS_t1, &pos_HS_dts);
	theta_e_ENC = pos_ENC_angle/PP;
}

float EstimateThetaHS(int pos_HS_state, float pos_HS_t1, float *pos_HS_dts){
//Estimate angle from hall sensors
return 0;
}
