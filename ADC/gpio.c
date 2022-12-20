#include "gpio.h"



void RGBLed_Init(void){
	
	// Activarea semnalului de ceas pentru pinii folositi în cadrul led-ului RGB
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK;
	
	// --- RED LED ---
	
	// Utilizare GPIO ca varianta de multiplexare
	PORTB->PCR[RED_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[RED_LED_PIN] |= PORT_PCR_MUX(1);
	
	// Configurare pin pe post de output
	GPIOB_PDDR |= (1<<RED_LED_PIN);
	
	// Stingerea LED-ului (punerea pe 0 logic)
	GPIOB_PCOR |= (1<<RED_LED_PIN);
	
	// --- GREEN LED ---
	
	// Utilizare GPIO ca varianta de multiplexare
	PORTB->PCR[GREEN_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[GREEN_LED_PIN] |= PORT_PCR_MUX(1);
	
	// Configurare pin pe post de output
	GPIOB_PDDR |= (1<<GREEN_LED_PIN);
	
	// Stingerea LED-ului (punerea pe 0 logic)
	GPIOB_PSOR |= (1<<GREEN_LED_PIN);
	
	// --- BLUE LED ---
	
		// Utilizare GPIO ca varianta de multiplexare
	PORTD->PCR[BLUE_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[BLUE_LED_PIN] |= PORT_PCR_MUX(1);
	
	// Configurare pin pe post de output
	GPIOD_PDDR |= (1<<BLUE_LED_PIN);
	
	// Stingerea LED-ului (punerea pe 0 logic)
	GPIOD_PSOR |= (1<<BLUE_LED_PIN);
}