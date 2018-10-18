#include <avr/io.h>
#include <avr/interrupt.h>
 
volatile unsigned char TimerFlag = 0;
 
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;
 
void TimerOn()
{
    TCCR1B = 0x0B;
    OCR1A = 125;
    TIMSK1 = 0x02;
    TCNT1 = 0;
   
    _avr_timer_cntcurr = _avr_timer_M;
    SREG |= 0x80;
}
 
void TimerOff()
{
    TCCR1B = 0x00;
}
 
void TimerISR()
{
    TimerFlag = 1;
}
 
ISR(TIMER1_COMPA_vect)
{
    _avr_timer_cntcurr--;
    if(_avr_timer_cntcurr == 0)
    {
        TimerISR();
        _avr_timer_cntcurr = _avr_timer_M;
    }
}
 
void TimerSet(unsigned long M)
{
    _avr_timer_M = M;
    _avr_timer_cntcurr = _avr_timer_M;
}
 
 
unsigned char receivedData = 0x22;
unsigned char pattern;
unsigned int speed_set;
unsigned char speed;
unsigned long period_length=40;
unsigned char counter=0x00;
unsigned char pattern_mode=1;
unsigned char time_mode=1;
 
unsigned char pattern1;
unsigned char pattern2;
unsigned char pattern3;
unsigned char pattern4;
 
 
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
    //receivedData = SPDR;
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
    switch (state){ //Transitions
        case Start:
        break;
        default:
        break;
    }
   
    switch (state){ //State Actions
        case Start:
        pattern_mode = (receivedData & 0xF0)>>4;
        time_mode = (receivedData & 0x0F);
        //pattern_mode = 1;
        //time_mode = 6;
       
        if(time_mode==1)
            period_length=1500;
        else if(time_mode==2)
            period_length=900;
        else if(time_mode==3)
            period_length=600;
        else if(time_mode==4)
            period_length=400;
        else if(time_mode==5)
            period_length=200;
        else if(time_mode==6)
            period_length=100;
       
        if(pattern_mode==1) {
            PORTA = pattern1;
        } else if(pattern_mode==2) {
            PORTA = pattern2;
        } else if(pattern_mode==3) {
            PORTA = pattern3;
        } else if(pattern_mode==4) {
            PORTA = pattern4;
        }
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
        sm1=state_12;
        break;
       
        case state_12:
        sm1=state_11;
        break;
        default:
        break;
    }
   
    switch(sm1){
        case state_11:
        pattern1=0xF0;
        break;
        case state_12:
        pattern1=0x0F;
        break;
        default:
        break;
    }
}
 
enum sm_pattern2{start2,state_21,state_22} sm2;
void Tick_SM2(){
    switch(sm2){
       
        case start2:
        sm2=state_21;
        break;
       
        case state_21:
        sm2=state_22;
        break;
       
        case state_22:
        sm2=state_21;
        break;
        default:
        break;
    }
   
    switch(sm2){
        case state_21:
        pattern2=0xAA;
        break;
        case state_22:
        pattern2=0x66;
        break;
        default:
        break;
    }
}
 
enum sm_pattern3{start3,state_31,state_32} sm3;
void Tick_SM3(){
    switch(sm3){
       
        case start3:
        sm3=state_31;
        break;
       
        case state_31:
        sm3=state_32;
        break;
       
        case state_32:
        sm3=state_31;
        break;
        default:
        break;
    }
   
    switch(sm3){
        case state_31:
        pattern3=0xAA;
        break;
        case state_32:
        pattern3=0x55;
        break;
        default:
        break;
    }
}
 
enum sm_pattern4{start4,state_41,state_42} sm4;
void Tick_SM4(){
    switch(sm4){
       
        case start4:
        sm4=state_41;
        break;
       
        case state_41:
        sm4=state_42;
        break;
       
        case state_42:
        sm4=state_41;
        break;
        default:
        break;
    }
   
    switch(sm4){
        case state_41:
        pattern4=0xAA;
        break;
        case state_42:
        pattern4=0x44;
        break;
        default:
        break;
    }
}
//old part4
/*
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
*/
 
int main(void)
{
    unsigned long STIME = 1000;
    unsigned long RTIME = 1000;
   
    const unsigned long timerPeriod = 100;
   
    DDRA = 0xFF;     PORTA = 0x00;
    //DDRB = 0x00;   PORTB = 0xFF;
   
    SPI_ServantInit();
   
    TimerSet(timerPeriod);
    TimerOn();
   
   
    /* Replace with your application code */
    while (1)
    {
        while(!TimerFlag);
        if (STIME >= period_length) {
            Tick_SM1();
            Tick_SM2();
            Tick_SM3();
            Tick_SM4();
            STIME = 0;
        }
       
        while(!TimerFlag);
        TimerFlag = 0;
        STIME += timerPeriod;
        RTIME += timerPeriod;
       
        Tick();
    }
}
