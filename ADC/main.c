#include "Adc.h"
#include "Uart.h"
#include "gpio.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern int ch;
extern int start;

float ambient_low=100.0f;
float ambient_high=0.0f;

float vibration_low=100.0f;
float vibration_high=0.0f;

int region=-1;

int set_led_green(){
	GPIOB_PSOR |= (1<<RED_LED_PIN);
	GPIOB_PCOR |= (1<<GREEN_LED_PIN);
	GPIOD_PSOR |= (1<<BLUE_LED_PIN);
}

int set_led_yellow(){
	GPIOB_PCOR |= (1<<RED_LED_PIN);
	GPIOB_PCOR |= (1<<GREEN_LED_PIN);
	GPIOD_PSOR |= (1<<BLUE_LED_PIN);
}

int set_led_red(){
	GPIOB_PCOR |= (1<<RED_LED_PIN);
	GPIOB_PSOR |= (1<<GREEN_LED_PIN);
	GPIOD_PSOR |= (1<<BLUE_LED_PIN);
}

void update_led(float f_value,float *low,float* high){
	float threshold_low_mid=*low+(*high-*low)/3.0f;
	float threshold_mid_high=*low+(*high-*low)*(2.0f/3.0f);
		
		if(*low<=f_value && f_value<threshold_low_mid){
			if(0!=region){
				set_led_green();
			}
			region=0;
		}
		
		if(threshold_low_mid<f_value && f_value<threshold_mid_high){
			if(1!=region){
				set_led_yellow();
			}
			region=1;
		}
		
		if(threshold_mid_high<f_value && f_value<*high){
			if(2!=region){
				set_led_red();
			}
			region=2;
		}
	
}

void update_limits(float f_value,float *low,float* high){
	if(f_value<*low){
		*low=f_value;
	}
	
	if(f_value>*high){
		*high=f_value;
	}
}

void send_buffer(unsigned char* buffer,int length){
	//big endian
	for(int i=0;i<length;i++){
			UART0_Transmit(buffer[i]);
	}
}


int main() {
	
	UART0_Init(115200);
	ADC0_Init();
	RGBLed_Init();
	
	
	//determinare endianness
	//uint16_t x=0x0205;
	//unsigned char* ptr=&x;
	//UART0_Transmit(*(ptr+1)+0x30);
	//return 0;
	
	//determinare size float
	//UART0_Transmit(sizeof(float)+0x30);	
	//return 0;
	
	for(;;) {
		if(start==0)
			continue;
		
		uint16_t ambient = ADC0_Read(ADC_AMBIENT_CHANNEL);
		float f_ambient=((ambient * 3.3f) / 65535);
		//print_voltage(f_ambient);
		send_buffer((unsigned char*)&f_ambient,4);
		
		//float valueDanut=2.076251;
		
		uint16_t vibration = ADC0_Read(ADC_VIBRATION_CHANNEL);
		float f_vibration=((vibration * 3.3f) / 65535);
		//print_voltage(f_vibration);
		send_buffer((unsigned char*)&f_vibration,4);
	
		
		update_limits(f_ambient,&ambient_low,&ambient_high);
		update_limits(f_vibration,&vibration_low,&vibration_high);
		

		if(ch==ADC_AMBIENT_CHANNEL){
			update_led(f_ambient,&ambient_low,&ambient_high);
		}else{
			update_led(f_vibration,&vibration_low,&vibration_high);
		}
		
		start=0;
		
	}
	
}
