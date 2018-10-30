#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"
#include "bit.h"

enum DemoStates{init, shift_up, shift_down, wait_up, wait_down} LEDstate;

	void transmit_data(unsigned char data) {
    /*
    SER = PORTA0;
    OE = PORTA1;
    RCLK = PORTA2;
    SRCLK = PORTA3;
    SRCLR = PORTA4;
    */
   
    // Ensure DDRA is setup as output
    DDRC = 0xFF;
   
    // When ready to begin transmitting, set SRCLR high and RCLK low.
    PORTC = SetBit(PORTC, 4, 1);
    PORTC = SetBit(PORTC, 2, 0);
   
    // for each bit of data
    for (int i = 0; i < 8; i++) {
        // Set SRCLR to 1 allowing data to be set
        //SetBit(PORTA, 4, 1);
        // Also clear SRCLK in preparation of sending data
        PORTC = SetBit(PORTC, 3, 0);
        // set SER = next bit of data to be sent.
        PORTC = SetBit(PORTC, 0, 1); //GetBit(data, i) );
        // set SRCLK = 1. Rising edge shifts next bit of data into the shift register
        PORTC = SetBit(PORTC, 3, 1);
    }
    // end for each bit of data
   
    // set RCLK = 1. Rising edge copies data from the "Shift" register to the "Storage" register
    PORTC = SetBit(PORTC, 2, 1);
    // clears all lines in preparation of a new transmission
    PORTC = SetBit(PORTC, 4, 0);
}
	void transmit_data1(unsigned char data)
	 {
    /*
    SER = PORTA0;
    OE = PORTA1;
    RCLK = PORTA2;P
    SRCLK = PORTA3;
    SRCLR = PORTA4;
    */
   
    // Ensure DDRA is setup as output
    DDRD = 0xFF;
   
    // When ready to begin transmitting, set SRCLR high and RCLK low.
    PORTD = SetBit(PORTD, 4, 1);
    PORTD = SetBit(PORTD, 2, 0);
   
    // for each bit of data
    for (int i = 0; i < 8; i++) {
        // Set SRCLR to 1 allowing data to be set
        //SetBit(PORTA, 4, 1);
        // Also clear SRCLK in preparation of sending data
        PORTD = SetBit(PORTD, 3, 0);
        // set SER = next bit of data to be sent.
        PORTD = SetBit(PORTD, 0, 1);//GetBit(data, i) );
        // set SRCLK = 1. Rising edge shifts next bit of data into the shift register
        PORTD = SetBit(PORTD, 3, 1);
    }
    // end for each bit of data
   
    // set RCLK = 1. Rising edge copies data from the "Shift" register to the "Storage" register
    PORTD = SetBit(PORTD, 2, 1);
    // clears all lines in preparation of a new transmission
    PORTD = SetBit(PORTD, 4, 0);
}
	void Demo_Tick()
	{
		static unsigned char pattern = 0xFF;
		static unsigned char row = 0xFE;
		switch(LEDstate)
		{
			case init:
			if(!GetBit(PINA, 0))
			LEDstate = wait_up;
			else if(!GetBit(PINA, 1))
			LEDstate = wait_down;
			else
			LEDstate = init;
			if(!GetBit(PORTD, 1))
			row = 0xEF;
			if(!GetBit(PORTD, 0))
			row = 0xFE;
			break;
			case wait_up:
			if(!GetBit(PINA, 0))
			LEDstate = wait_up;
			else
			LEDstate = shift_up;
			break;
			case wait_down:
			if(!GetBit(PINA,1))
			LEDstate = wait_down;
			else
			LEDstate = shift_down;
			break;
			case shift_up:
			LEDstate = init;
			break;
			case shift_down:
			LEDstate = init;
			break;
			default:
			LEDstate = init;
			break;
			
		}
		switch(LEDstate)
		{
			case init:
			break;
			case wait_up:
			break;
			case wait_down:
			break;
			case shift_up:
			if( GetBit(PORTD,0) )
			row = ~(~row >> 1);
			break;
			case shift_down:
			if( GetBit(PORTD,4) )
			row = ~(~row << 1);
			break;
			default:
			break;
		}
		transmit_data(pattern);		// Pattern to display
		transmit_data1(row);		// Row(s) displaying pattern
	}
	void Shift_Task()
	{
		LEDstate = init;
		for(;;)
		{
			Demo_Tick();
			vTaskDelay(250);
		}
	}
	void StartShiftPulse(unsigned portBASE_TYPE Priority)
	{
		xTaskCreate(Shift_Task, (signed portCHAR *)"Shift_Task", configMINIMAL_STACK_SIZE, NULL, Priority, NULL);
	}
	int main(void)
	{
		DDRA = 0xFF; PORTA = 0xFF;
		DDRC = 0xFF; PORTC = 0x00;
		DDRD = 0xFF; PORTD = 0x00;
		
		StartShiftPulse(1);
		vTaskStartScheduler();
		return 0;
	}
