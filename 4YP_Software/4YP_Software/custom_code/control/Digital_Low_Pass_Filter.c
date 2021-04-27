/*
 * Digital_Low_Pass_Filter.c
 *
 * Created: 25/04/2021 15:34:17
 *  Author: Ruzhev
 */ 



#include "Digital_Low_Pass_Filter.h"
#include <stdlib.h>

//add these 3 lines before every instance of arm_math.h ;
//configuration macro to enable the correct code within the library
#ifndef ARM_MATH_CM7
#define ARM_MATH_CM7
#endif
//always have atmel_start.h before arm_math.h -> seems to be fixing some issiues
#include <atmel_start.h>
#include "arm_math.h"



void DLPF_Init(struct DLPF* LPF, int order, float cutoff_freq, float sampling_freq){
	//index 0 <=> z^-1, index 1 <=> z^-2 etc
	//some of the arrays are one larger than they need to be but that is worth the ease of programming
	LPF->previous_inputs	= (float *) malloc((order+1)*sizeof(float));
	LPF->previous_outputs	= (float *) malloc((order+1)*sizeof(float));

	LPF->order = order;
	LPF->array_size = order + 1;
	
	
	
	if (LPF->order == 1){
		LPF->coef_input			= (float *) malloc((order+1)*sizeof(float));
		LPF->coef_output		= (float *) malloc((order+1)*sizeof(float));
		
		float temp = sampling_freq / (PI * cutoff_freq);
		float common_denom = 1+temp;
		LPF->coef_input[0] = 1 / (common_denom);
		LPF->coef_input[1] = 1 / (common_denom);
		
		LPF->coef_output[0] = 0;
		LPF->coef_output[1] = (temp-1) / (common_denom);
	}
	
	//orders higher than two not supported, but it is possible to input the values manually
	if (LPF->order >= 2){
		//second order butterworth filter
		LPF->order = 2;
		
		LPF->coef_input			= (float *) malloc((order+1)*sizeof(float));
		LPF->coef_output		= (float *) malloc((order+1)*sizeof(float));
		
		float temp = sampling_freq / (PI * cutoff_freq);
		float zeta = 1.4142;	//sqrt 2
		
		float common_denom = temp*temp + zeta *temp + 1;
		LPF->coef_input[0] = 1 / (common_denom);
		LPF->coef_input[1] = 2 / (common_denom);
		LPF->coef_input[2] = 1 / (common_denom);
		
		LPF->coef_output[0] = 0;
		LPF->coef_output[1] = - (2 - 2*temp*temp) / (common_denom);
		LPF->coef_output[2] = - (temp*temp - zeta *temp + 1) / (common_denom);
	}
	
	//DLPF_Print_Coeff(LPF);
	
}

float DLPF_Filter(struct DLPF* LPF, float input){
	//there is a more efficient way of doing this whole calculation, but it shouldnt matter in this case
	LPF->previous_inputs[0] = input;
	
	float output =0;
	for (int i=0;i <= LPF->order;i++){
		output += LPF->previous_inputs[i] * LPF->coef_input[i];
		output += LPF->previous_outputs[i] * LPF->coef_output[i];
	}
	
	LPF->previous_outputs[0] = output;
	
	//shift values back by one z
	for(int i=LPF->array_size-1; i>0; i--){
		LPF->previous_inputs[i] = LPF->previous_inputs[i-1];
		LPF->previous_outputs[i] = LPF->previous_outputs[i-1];
	}
	
	return output;
}

void DLPF_Print_IO(struct DLPF* LPF){
	printf("LPF_Inputs = ");
	for(int i=0;i<LPF->array_size;i++) printf("%f \t ",LPF->previous_inputs[i]);
	printf("\nLPF_Outputs = ");
	for(int i=0;i<LPF->array_size;i++) printf("%f \t ",LPF->previous_outputs[i]);
	printf("\n");
}

void DLPF_Print_Coeff(struct DLPF* LPF){
	printf("LPF_Input_coeff = ");
	for(int i=0;i<=LPF->order;i++) printf("%f \t ",LPF->coef_input[i]);
	printf("\nLPF_Output_coeff = ");
	for(int i=0;i<=LPF->order;i++) printf("%f \t ",LPF->coef_output[i]);
	printf("\n");
}