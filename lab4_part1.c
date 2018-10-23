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
enum BlinkLeds {INIT,TOGGLE} led_state;

unsigned char temp = 0x15;

void LEDS_Init(){
	led_state = INIT;
	PORTD = temp;
}

void LEDS_Tick(){
	//Transitions
	switch(led_state){
		case INIT:
		led_state = TOGGLE;
		break;
		
		case TOGGLE:
		break;
		
		default:
		led_state = INIT;
		break;
	}
	//Actions
	switch(led_state){
		case INIT:
		break;
		
		case TOGGLE:
		temp ^= 0x15;
		PORTD = temp;
		break;
		
		default:
		break;
	}
}

void LedSecTask()
{
	LEDS_Init();
	for(;;)
	{
		LEDS_Tick();
		vTaskDelay(1000);
	}
}

void StartSecPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(LedSecTask, (signed portCHAR *)"LedSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
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
