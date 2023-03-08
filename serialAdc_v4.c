#include <12F675.h>
#device ADC=10
#fuses INTRC_IO,NOWDT,NOPUT,NOPROTECT,NOCPD,NOMCLR
#use delay(INTERNAL=4000000)

#use rs232(baud=9600, xmit=PIN_A4, rcv=PIN_A3)

#include <stdio.h>

#define GP0 PIN_A0
#define GP1 PIN_A1
#define GP2 PIN_A2
#define GP3 PIN_A3
#define GP4 PIN_A4
#define GP5 PIN_A5
#byte OSCCAL = 0x90

//unsigned int16 volt=10;//,delaytriac=2500;


unsigned int32 value;
unsigned int32 volt1;

void init()
{

    set_tris_a(0b11101001); // set gp1,gp2 as output; GP5 input, 1 indicates the pin is input 
port_a_pullups(0b11101001); // 1 = enable pullup on inputs
setup_comparator( NC_NC_NC_NC ); // disable comparators

//set_adc_reference(VSS_VDD);
setup_adc(ADC_CLOCK_INTERNAL);
setup_adc_ports(PIN_A0); 
set_analog_pins(PIN_A0);
set_adc_channel(0);
//the next read_adc call will read channel 0
 
delay_us(10);
output_low(GP1); // turn triac off
output_low(PIN_A1);
output_low(PIN_A2);

//setup_adc( ADC_OFF ); // disable A2D

//output_float(PIN_A2); // Make pin GP2 into an input
output_float(GP5); 
}

unsigned int32 readCurrent()
{ 
   unsigned int32 volt;
   
   value=read_adc();
   volt1 = value * 480;
   volt = volt1 / 1023;
   return(volt);
}

void onMotor()
{
unsigned int32 currentVal;
unsigned int16 count=0;

output_high(PIN_A1);
delay_ms(1000);   
   while(input_state(pin_A1)){
   currentVal = readCurrent();
   if(currentVal<=200 || count>=30000){
      output_low(PIN_A1);
   }
   printf("Current: %Lu \n\r",currentVal);
   delay_ms(1);
   count++;
   }

}

void main()
{

unsigned int32 currentVal,count=0,currentMil,prevMil=0;

init();

delay_ms( 2000 ); // wait 1s

while ( TRUE ) // blink LED
{

if( !input(PIN_A5) ){
   output_low(PIN_A2);   //motor direction
   delay_ms(2000);
   onMotor();
   delay_ms(2000);
   while(!input(PIN_A5));
   
   output_high(PIN_A2);  //motor direction
   delay_ms(2000);
   onMotor();
   delay_ms(1000);
   output_low(PIN_A2);
}

//========================================
currentMil=count;
if(currentMil-prevMil>=1000){
   currentVal = readCurrent();
   printf("ADC: %Lu ",value);
   printf("Volt1: %Lu volt2: %Lu\n\r",volt1,currentVal);
   prevMil=currentMil;
}

//delay_ms(1000);
delay_ms(1);
count++;

}
}
