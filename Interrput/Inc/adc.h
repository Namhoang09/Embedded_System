#ifndef __ADC_H
#define __ADC_H

#include "stm32f4xx.h"

#define VREF 	3.3f
#define RL 		1.0f
#define R0 		1.9f
#define A 		-0.48f
#define B 		1.33f

void ADC1_Init(void);
void ADC1_Start(void);
uint16_t MQ2_Read_PPM(void);

extern volatile uint16_t adc_result;
extern volatile uint16_t latest_ppm;
extern volatile uint8_t new_adc_ready;

#endif
