#include <avr/io.h>
#include <scheduler.h>
 
unsigned char receivedData;
unsigned char pattern;
unsigned int speed_set;
unsigned char speed;
 
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
   
    }
}
