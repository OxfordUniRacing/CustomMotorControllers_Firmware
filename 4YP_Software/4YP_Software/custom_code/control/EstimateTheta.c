/*
 * EstimateTheta.c
 *
 * Created: 18/01/2021 14:08:16
 *  Author: Ben
 */ 

#include "EstimateTheta.h"
#include "ControlConfig.h"

float AnglesENC[200];
float AnglesHS[200];
int counting;

float EstimateTheta(int pos_HS_state, float pos_HS_t1, float *pos_HS_dts, float pos_ENC_angle){
	//Estimate angle from combination of maybe encoder and maybe observer and maybe hall sensors
	theta_e_HS = EstimateThetaHS(pos_HS_state, pos_HS_t1, pos_HS_dts);
	theta_e_ENC = pos_ENC_angle*(PP*GR);
	
	//Get the data for analysis.. Comment this bit out for normal operation
	counting++;
	if(counting < 10200 && 10000 <= counting){
		AnglesENC[counting-10000] = theta_e_ENC;
		AnglesHS[counting-10000] = theta_e_HS;		
	}
	if (counting == 15000)
	{
		counting = 0;
		printf("\n Encoder Angle = ");
		for (int i = 0; i<200; i++)
		{
			printf("\t %f", AnglesENC[i]);
		}
		
		printf("\n Algorithm Angle = ");
		for (int i = 0; i<200; i++)
		{
			printf("\t %f", AnglesHS[i]);
		}
		
		printf("\n");
	}

	return theta_e_ENC;
}

float EstimateThetaHS(int pos_HS_state, float pos_HS_t1, float *pos_HS_dts){
//Estimates angle based on hall sensors. Times are in microseconds, how they are passed is in positionsensors.c
float Angle = (pos_HS_state - 1.0 + pos_HS_t1/pos_HS_dts[0] + 0.5*(1.0/pos_HS_dts[1] - 1.0/pos_HS_dts[0])*pos_HS_t1*pos_HS_t1)*PI_OVER_3;
return Angle;
}
