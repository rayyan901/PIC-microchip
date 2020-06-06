#include <12F675.h>
#device ADC=10
#fuses INTRC_IO,NOWDT,NOPUT,NOPROTECT,NOCPD,NOMCLR
#use delay(INTERNAL=4000000)

#use rs232(baud=9600, xmit=PIN_A4, rcv=PIN_A3)


#define GP0 PIN_A0
#define GP1 PIN_A1
#define GP2 PIN_A2
#define GP3 PIN_A3
#define GP4 PIN_A4
#define GP5 PIN_A5
#byte OSCCAL = 0x90

int16 triacDelay=8000;//,delaytriac=2500;






#int_ext
void int_ext_isr()
{
   
   delay_us(triacDelay);  
   output_high( GP1 ); // turn triac on
   delay_us(500);
   output_low( GP1 ); // turn triac off

}


void init()
{
//OSCCAL = 0x80; // set internal oscillator to mid frequency
   set_tris_a( 0b11101101 ); // set GP1 output, all other inputs
port_a_pullups(0b11101101); // enable pullup on inputs
setup_comparator( NC_NC_NC_NC ); // disable comparators
setup_adc(ADC_CLOCK_INTERNAL);
//set_adc_reference(VSS_VDD);
setup_adc_ports(PIN_A0);
 
 set_analog_pins(PIN_A0);
set_adc_channel(0);
 //the next read_adc call will read channel 0
 
delay_us(10);
output_low(GP1); // turn triac off

//setup_adc( ADC_OFF ); // disable A2D

output_float(PIN_A2); // Make pin GP2 into an input
ext_int_edge(L_TO_H);
enable_interrupts(INT_EXT);
enable_interrupts(GLOBAL);
}
void main()
{
//int i;
int16 value;//,count=0;
float valueinFloat;
init();
//servo(1,550);
delay_ms( 2000 ); // wait 1s

while ( TRUE ) // blink LED
{

value=read_adc();
valueinFloat=value*1.0;
valueinFloat=(valueinFloat/1023)*8500;
triacDelay=valueinFloat;

printf("ADC: %Lu Triac: %Lu \n\r",value,triacDelay);
delay_ms(500);

}
}

