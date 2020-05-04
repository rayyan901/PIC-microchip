#include <12F675.h>
#device ADC=10
#fuses INTRC_IO,NOWDT,NOPUT,NOPROTECT,NOCPD,NOMCLR
#use delay(INTERNAL=4000000)
#define GP0 PIN_A0
#define GP1 PIN_A1
#define GP2 PIN_A2
#define GP3 PIN_A3
#define GP4 PIN_A4
#define GP5 PIN_A5
//========================osccal=3440

#use fast_io(a)
#byte ADCON0 = 0x1F
#byte ANSEL = 0x9F
#byte CMCON = 0x19
//#rom 0x3ff = {0x3470} // input the calibration code

int16 rtcc=0,timerVal=0xffb8;  //e0=14ms d0=15ms    c0=18ms b9=18.8ms b7=20.2ms
int16 poswidth=10;

//====================
#INT_TIMER1
void wave_timer() {
   int i;

   set_timer1(timerVal);  //100 us rollover  
   if(rtcc==0){
   output_high(GP1);
   }
   if(rtcc==poswidth)  //high width
   {
   output_low(GP1);
   }
   rtcc=rtcc+1;  // set_timer0(99);

   if(rtcc==200){
   rtcc=0;
   }
   
   
}
//=========
void main()
{

ADCON0 = 0; // ADC off
ANSEL = 0; // GPIO pins 0,1,2 and 4 set to all digital
CMCON = 7; // Comparators off
rtcc=0;
output_low(GP1);
delay_ms(100);


 setup_timer_1(T1_INTERNAL|T1_DIV_BY_1);   // setup interrupts
   enable_interrupts(INT_TIMER1);
   enable_interrupts(GLOBAL);
   set_timer1(timerVal);  //100 us rollover 
 
set_tris_a( 0b11111101 ); // set GP1 output, all other inputs
port_a_pullups(0b11111101); // enable pullup on inputs
poswidth=21;   // 500us = 0 deg  1.3ms=91 deg
//output_low(GP0);
while(1) {
//output_high(GP1);
delay_ms(250);
//!output_low(GP1);
//!delay_ms(250);
  }

}
