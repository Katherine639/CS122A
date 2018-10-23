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
enum BlinkLed {INIT,TOGGLE} led_state;

unsigned char output3 = 0x01;
unsigned char CNT_DIR = 0;

void LEDS_Init(){
	led_state = INIT;
	PORTD = 0;
}
void LED_Tick(){
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
		if(CNT_DIR == 1)
		{
			if(output3 == 0x01)
			{
				CNT_DIR = 0;
				break;
			}
			else
			{
				output3 >>= 1;
			}
		}
		else if(CNT_DIR == 0)
		{
			if(output3 == 0x80)
			{
				CNT_DIR = 1;
				break;
			}
			else
			{
				output3 <<= 1;
			}
		}
		PORTD = output3;
		break;
		
		default:
		break;
	}
}

void LedSecTask()
{
	LEDS_Init();
	while(1)
	{
		LED_Tick();
		vTaskDelay(200);

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
