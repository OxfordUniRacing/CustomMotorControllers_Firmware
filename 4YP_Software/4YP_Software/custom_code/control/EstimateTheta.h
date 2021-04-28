/*
 * EstimateTheta.h
 *
 * Created: 18/01/2021 14:08:03
 *  Author: Ben
 */ 


#ifndef GETTHETA_H_
#define GETTHETA_H_
#define PI_OVER_3 1.0471975512

float EstimateTheta(int pos_HS_state, float pos_HS_t1, float *pos_HS_dts, float pos_ENC_angle);
float EstimateThetaHS(int pos_HS_state, float pos_HS_t1, float *pos_HS_dts);
float theta_e_HS;
float theta_e_ENC;




#endif /* GETTHETA_H_ */

