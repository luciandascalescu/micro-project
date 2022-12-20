#include "MKL25Z4.h"

#define ADC_AMBIENT_CHANNEL (12) // PORT B PIN 1
#define ADC_VIBRATION_CHANNEL (11) // PORT C PIN 2

void ADC0_Init(void);
int ADC0_Calibrate(void);
uint16_t ADC0_Read(uint32_t channel);
void ADC0_IRQHandler(void);

void print_voltage(float);
