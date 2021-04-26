/*
 * Digital_Low_Pass_Filter.h
 *
 * Created: 25/04/2021 15:33:53
 *  Author: Ruzhev
 */ 


#ifndef DIGITAL_LOW_PASS_FILTER_H_
#define DIGITAL_LOW_PASS_FILTER_H_

struct DLPF{
	int order;
	int array_size;
	//index 0 <=> z^-1, index 1 <=> z^-2 etc
	float * previous_inputs;
	float * previous_outputs;
	float * coef_input;
	float * coef_output;
};

void DLPF_Init(struct DLPF* LPF, int order, float cutoff_freq, float sampling_freq);
float DLPF_Filter(struct DLPF* LPF, float input);

void DLPF_Print_IO(struct DLPF* LPF);
void DLPF_Print_Coeff(struct DLPF* LPF);



#endif /* DIGITAL_LOW_PASS_FILTER_H_ */