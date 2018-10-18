#include <avr/io.h>
#include <avr/interrupt.h>
#include <scheduler.h>


unsigned char receivedData;
unsigned char pattern;
unsigned int speed_set;
unsigned char speed;
unsigned char period_length=40;
unsigned char counter=0x00;
unsigned char pattern_mode=1;
unsigned char time_mode=1;

void SPI_ServantInit(void) {
	// set DDRB to have MISO line as output and MOSI, SCK, and SS as input
	DDRB = 0x4F; PORTB = 0xB0;
	// set SPCR register to enable SPI and enable SPI interrupt (pg. 168)
	SPCR = (1<<SPE);
	// make sure global interrupts are enabled on SREG register (pg. 9)
	SREG |= 0x80;
}

ISR(SPI_STC_vect) { // this is enabled in with the SPCR register’s “SPI
	// this is enabled in with the SPCR register’s “SPI Interrupt Enable”
	while(!(SPSR & (1<<SPIF)))
	;
	// SPDR contains the received data, e.g. unsigned char receivedData = SPDR;
	return SPDR;
}
/*

enum sm_2 {sm2_a, sm2_b};
int SMP2(int state)
{
	//Transitions
	switch(state)
	{
		case -1:
		PORTA = 0xAA;
		state = sm2_a;
		break;
		case sm2_a:
		state = sm2_b;
		break;
		case sm2_b:
		state = sm2_a;
		break;
		default:
		state = -1;
		break;
	}
	
	//Actions
	switch(state)
	{
		case sm2_a:
		PORTA = 0XAA;
		break;
		case sm2_b:
		PORTA = 0x55;
		break;
		default:
		break;
	}
	return state;
}
*/
enum States{Start} state;
void Tick(){
	switch (state){	//Transitions
		
		case Start:
		break;
		
		default:
		break;
		
	}
	
	switch (state){	//State Actions
		
		case Start:
		//receivedData = SPI_Transmit(testChar);
		pattern_mode=receivedData&0x0F;
		time_mode=(receivedData&0xF0)>>4;
		if(time_mode==1)
		period_length=40;
		else if(time_mode==2)
		period_length=20;
		else if(time_mode==3)
		period_length=10;
		else if(time_mode==4)
		period_length=5;
		else if(time_mode==5)
		period_length=2;
		else if(time_mode==6)
		period_length=1;
		break;
		
		default:
		break;
	}
}

enum sm_pattern1{start1,state_11,state_12} sm1;
void Tick_SM1(){
	switch(sm1){
		
		case start1:
		sm1=state_11;
		break;
		
		case state_11:
		if(counter<period_length)
		counter++;
		else{
			counter=0;
			sm1=state_12;
		}
		break;
		
		case state_12:
		if(counter<period_length)
		counter++;
		else{
			counter=0;
			sm1=state_11;
		}
		break;
		
		default:break;
	}
	switch(sm1){
		case state_11:
		PORTA=0xF0;
		break;
		case state_12:
		PORTA=0x0F;
		break;
		default:break;
	}
}

enum sm_pattern2{start2,state_21,state_22} sm2;
void Tick_SM2(){
	switch(sm2){
		
		case start2:
		sm2=state_21;
		break;
		
		case state_21:
		if(counter<period_length)
		counter++;
		else{
			counter=0;
			sm2=state_22;
		}
		break;
		
		case state_22:
		if(counter<period_length)
		counter++;
		else{
			counter=0;
			sm2=state_21;
		}
		break;
		default:break;
	}
	
	switch(sm2){
		case state_21:
		PORTA=0xAA;
		break;
		case state_22:
		PORTA=0x66;
		break;
		default:break;
	}
}




enum sm_pattern3{start3,state_31,state_32} sm3;
void Tick_SM3(){
	switch(sm3){
		case start3:
		sm3=state_31;
		break;
		case state_31:
		if(counter<period_length)
		counter++;
		else{
			counter=0;
			sm3=state_32;
		}
		break;
		case state_32:
		if(counter<period_length)
		counter++;
		else{
			counter=0;
			sm3=state_31;
		}
		break;
		default:break;
	}
	switch(sm3){
		case state_31:
		PORTA=0xAA;
		break;
		case state_32:
		PORTA=0x55;
		break;
		default:break;
	}
}
enum sm_pattern4{start4,state_41,state_42,state_43,state_44,state_45,state_46,state_47,state_48} sm4;
void Tick_SM4(){
	switch(sm4){
		
		case start4:
		sm4=state_41;
		break;
		
		case state_41:
		if(counter<period_length)
		counter++;
		else{
			counter=0;
			sm4=state_42;
		}
		break;
		
		case state_42:
		if(counter<period_length)
		counter++;
		else{
			counter=0;
			sm4=state_43;
		}
		break;
		
		case state_43:
		if(counter<period_length)
		counter++;
		else{counter=0;
			sm4=state_44;
		}
		break;
		
		case state_44:
		if(counter<period_length)
		counter++;
		
		else{ counter=0;
		sm4=state_45;}
		
		break;
		
		case state_45:
		if(counter<period_length)
		counter++;
		else{counter=0;
		sm4=state_46;}
		
		break;
		
		
		case state_46:
		if(counter<period_length)
		counter++;
		else{
			counter=0;
			sm4=state_47;
		}
		break;
		case state_47:
		if(counter<period_length)
		counter++;
		else{
			counter=0;
			sm4=state_48;
		}
		break;
		case state_48:
		if(counter<period_length)
		counter++;
		else{
			counter=0;
			sm4=state_41;
		}
		break;
		default:break;
	}
	switch(sm4){
		case state_41:
		PORTA=0x01;
		break;
		case state_42:
		PORTA=0x02;
		break;
		case state_43:
		PORTA=0x04;
		break;
		case state_44:
		PORTA=0x08;
		break;
		case state_45:
		PORTA=0x10;
		break;
		case state_46:
		PORTA=0x20;
		break;
		case state_47:
		PORTA=0x40;
		break;
		case state_48:
		PORTA=0x80;
		break;
		default:break;
	}
}

void getPattern()
{
	pattern = (receivedData & 0xF0); //4 Possible Patterns
	speed = (receivedData & 0x0F);  //6 Possible Speeds
	
	//Check the pattern chosen
	if(pattern == 0x10)  //A
	{
		pattern = 0xF0;
	}
	
	if(pattern == 0x20)  //B
	{
		pattern = 0xAA;
	}
	
	if(pattern == 0x40)  //C
	{
		pattern = 0x80;
	}
	
	if(pattern == 0x80)  //D
	{
		pattern = 0x55;
	}
	
	
	//Check Speed Chosen
	
	if(speed == 0x01)  //1
	{
		speed_set = 2000;
	}
	
	if(speed == 0x02)  //2
	{
		speed_set = 1000;
	}
	
	if(speed == 0x03)  //3
	{
		speed_set = 500;
	}
	
	if(speed == 0x04)  //4
	{
		speed_set = 250;
	}
	
	if(speed == 0x05)  //5
	{
		speed_set = 100;
	}
	
	if(speed == 0x06)  //6
	{
		speed_set = 50;
	}
	
}
int main(void)
{
	DDRA = 0xFF;     PORTA = 0xAA;
	//DDRB = 0x00;   PORTB = 0xFF;
	
	SPI_ServantInit();
	
	tasksNum = 1; // declare number of tasks
	task tsks[1]; // initialize the task array
	tasks = tsks; // set the task array
	
	// define tasks
	unsigned char i=0; // task counter
	tasks[i].state = -1;
	tasks[i].period = 500;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &SMP2;
	
	TimerSet(50); // value set should be GCD of all tasks
	TimerOn();
	
	
	/* Replace with your application code */
	while (1)
	{
		Tick();
		
		switch(pattern_mode){
			case 1:
			Tick_SM1();
			break;
			case 2:
			Tick_SM3();
			break;
			case 3:
			Tick_SM4();
			break;
			case 4:
			Tick_SM2();
			break;
			default:break;
		}
		
	}
}
