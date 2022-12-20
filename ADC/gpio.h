#include "MKL25Z4.h"

#define SWITCH_PIN (12) // PORT A
#define RED_LED_PIN (18) // PORT B
#define GREEN_LED_PIN (19) // PORT B
#define BLUE_LED_PIN (1) // PORT D

void Switch_Init(void);
void RGBLed_Init(void);
void PORTA_IRQHandler(void);
void PORTD_IRQHandler(void);

