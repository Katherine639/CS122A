#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/portpins.h>
#include <avr/pgmspace.h>

//FreeRTOS include files
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"
enum BlinkLed0 {INIT0,TOGGLE0} led_state0;
enum BlinkLed2 {INIT2,TOGGLE2} led_state2;
enum BlinkLed4 {INIT4,TOGGLE4} led_state4;

unsigned char temp0 = 0x00;
unsigned char temp2 = 0x00;
unsigned char temp4 = 0x00;

void LEDS_Init0(){
	led_state0 = INIT0;
	PORTD = 0;
}
void LED0_Tick(){
	//Transitions
	switch(led_state0){
		case INIT0:
		led_state0 = TOGGLE0;
		break;
		
		case TOGGLE0:
		break;
		
		default:
		led_state0 = INIT0;
		break;
	}
	//Actions
	switch(led_state0){
		case INIT0:
		break;
		
		case TOGGLE0:
		PORTD ^= 0x01;
		break;
		
		default:
		break;
	}
}

void LEDS_Init2(){
	led_state2 = INIT2;
	PORTD = 0;
}
void LED2_Tick(){
	//Transitions
	switch(led_state2){
		case INIT2:
		led_state2 = TOGGLE2;
		break;
		
		case TOGGLE2:
		break;
		
		default:
		led_state2 = INIT2;
		break;
	}
	//Actions
	switch(led_state2){
		case INIT2:
		break;
		
		case TOGGLE2:
		PORTD ^= 0x04;
		break;
		
		default:
		break;
	}
}

void LEDS_Init4(){
	led_state4 = INIT4;
	PORTD = 0;
}
void LED4_Tick(){
	//Transitions
	switch(led_state4){
		case INIT4:
		led_state4 = TOGGLE4;
		break;
		
		case TOGGLE4:
		break;
		
		default:
		led_state4 = INIT4;
		break;
	}
	//Actions
	switch(led_state4){
		case INIT4:
		break;
		
		case TOGGLE4:
		PORTD ^= 0x10;
		break;
		
		default:
		break;
	}
	vTaskDelay(2500);
}

void LedSecTask0()
{
	LEDS_Init0();
	while(1)
	{
		LED0_Tick();
		vTaskDelay(500);

	}
}

void LedSecTask2()
{
	LEDS_Init2();
	while(1)
	{
		LED2_Tick();
		vTaskDelay(1000);

	}
}

void LedSecTask4()
{
	LEDS_Init4();
	while(1)
	{
		LED4_Tick();
		vTaskDelay(2500);

	}
}

void StartSecPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(LedSecTask0, (signed portCHAR *)"LedSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
	xTaskCreate(LedSecTask2, (signed portCHAR *)"LedSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
	xTaskCreate(LedSecTask4, (signed portCHAR *)"LedSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}

int main(void)
{
	DDRA = 0x00; PORTA=0xFF;
	DDRD = 0xFF;
	//Start Tasks
	StartSecPulse(1);
	//RunSchedular
	vTaskStartScheduler();
	
	return 0;
}
