/*
 * EstimateTheta.c
 *
 * Created: 18/01/2021 14:08:16
 *  Author: Ben
 */ 

#include "EstimateTheta.h"
#include "ControlConfig.h"

//#include <atmel_start.h>


// #define ANGLE_TEST_NUM_POINTS 300
// float AnglesENC[ANGLE_TEST_NUM_POINTS];
// float AnglesHS[ANGLE_TEST_NUM_POINTS];
// int counting;4



float EstimateTheta(int pos_HS_state, float pos_HS_t1, float *pos_HS_dts, float pos_ENC_angle){
	//Estimate angle from combination of maybe encoder and maybe observer and maybe hall sensors
	theta_e_HS = EstimateThetaHS(pos_HS_state, pos_HS_t1, pos_HS_dts);
	theta_e_ENC = pos_ENC_angle*(PP*GR);
	
	//Get the data for analysis.. Comment this bit out for normal operation
// 	counting++;
// 	if(counting < 30000 +ANGLE_TEST_NUM_POINTS*10 && 30000 <= counting){
// 		AnglesENC[(counting-30000)/10] = theta_e_ENC;
// 		AnglesHS[(counting-30000)/10] = theta_e_HS;		
// 	}
// 	if (counting == 45000)
// 	{
// 		counting = 0;
// 		printf("\n Encoder Angle = ");
// 		for (int i = 0; i<ANGLE_TEST_NUM_POINTS; i++)
// 		{
// 			printf("\t %f", AnglesENC[i]);
// 		}
// 		
// 		printf("\n Algorithm Angle = ");
// 		for (int i = 0; i<ANGLE_TEST_NUM_POINTS; i++)
// 		{
// 			printf("\t %f", AnglesHS[i]);
// 		}
// 		
// 		printf("\n");
// 	}

	return theta_e_ENC;
}

float EstimateThetaHS(int pos_HS_state, float pos_HS_t1, float *pos_HS_dts){
	//Estimates angle based on hall sensors. Times are in microseconds, how they are passed is in positionsensors.c
	if(pos_HS_dts[0] == 0 || pos_HS_dts[1] == 0) return 0;
	
	float Angle0 = (pos_HS_state - 1.0)*PI_OVER_3;
	float Angle1 = (pos_HS_t1/pos_HS_dts[0])*PI_OVER_3;
	float Angle2 = ((1.0/pos_HS_dts[1] - 1.0/pos_HS_dts[0])/(pos_HS_dts[0] + pos_HS_dts[1]))*pos_HS_t1*pos_HS_t1*PI_OVER_3;
	float Angle = (Angle0 + Angle1 + Angle2);
	if ((Angle)>(Angle0 + PI_OVER_3))
	{
		Angle = Angle0 + PI_OVER_3;
	}
	//printf("safsd %f \t %f\n",pos_HS_t1 , pos_HS_dts[0]);
	return Angle;
}
