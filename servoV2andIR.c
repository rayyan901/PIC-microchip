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


void servo(int pulseCount, int16 width){
int16 pulses;
int i;

pulses=pulseCount*50;

   for(i=0;i<pulses;i++){
   output_high( GP1 ); // turn LED on
   delay_us( width );
   output_low( GP1 ); // turn LED off
   delay_us( (20000-width) );
   }
}

void init()
{
//OSCCAL = 0x80; // set internal oscillator to mid frequency
set_tris_a( 0b11111101 ); // set GP1 output, all other inputs
port_a_pullups(0b11111101); // enable pullup on inputs
setup_comparator( NC_NC_NC_NC ); // disable comparators
setup_adc_ports( NO_ANALOGS ); // disable analog inputs
setup_adc( ADC_OFF ); // disable A2D
}
void main()
{

init();
servo(1,550);
delay_ms( 2000 ); // wait 1s

while ( TRUE ) // blink LED
{

if (input(Pin_A2))
{
 servo(2,1350); // 90 degree=1.35 ms
 servo(1,500);
 delay_ms(300);
   
}

}
}

