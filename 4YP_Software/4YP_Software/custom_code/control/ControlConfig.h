/*
 * ControlConfig.h
 *
 * Created: 28/02/2021 18:16:05
 *  Author: Ben
 */ 


#ifndef CONTROLCONFIG_H_
#define CONTROLCONFIG_H_


#define GR 5	//Gear Ratio
#define PP 15
#define FLUX_PM 0.0085						//Calculated from experiments 
#define LQ_TAKE_LD 0.0000082
//#define LST_SQ_OMEGA_BASE_E 37.62
#define I_MAX 10 //max current amps
#define C1 259.1463				//FLUX_PM/4(LQ - LD)
#define C1_SQR 67157				//C1 squared
#define T_RATE_UP 0.0001					//T_RATE 0.00666 = 1000Nm/s (torque limited rate of change) * Ts (1/switching frequency)
#define T_RATE_DOWN 0.0001					//T_RATE 0.00666 = 1000Nm/s (torque limited rate of change) * Ts (1/switching frequency)


float oldtorquerequest;




#endif /* CONTROLCONFIG_H_ */