#include "isr.h"

#include "p33FJ64GS406.h"
#include <libq.h>
#include <dsp.h>
#include <stdint.h>

#include "ReadingModel.h"
#define refv 2.4f

float kp=0.00021231;
float TsTi=4.2;

float WR0, WR1, WR2, WR3, WR4, WR5, WR6, WR7, WR8, WR9;
float refi;
//float ADC0value;
float ADC1value;
int x=0;


int readings_updated = 0;
uint16_t battery_output_current_adc = 0;
uint16_t battery_input_current_adc = 0;
uint16_t battery_voltage_adc = 0;
uint16_t network_voltage_adc = 0;

void __attribute__((__interrupt__, no_auto_psv)) _ADCP1Interrupt()
{
    TRISFbits.TRISF6 = 0;

    LATFbits.LATF6 = 1;
 //   WREG0=1*2.2;
 //   WREG1=2.8;
 
   //WREG0 x;  WREG1 x1; WREG2 y1 WREG3 y2 WREG4 y
   // AT=_Q16mac(AW,AZ,AT);
  //  FAT=  _itofQ16((_Q16)AZ);
 //   AAT2= _Q16ftoi(FAT2);
//    FAT3=  _itofQ16((_Q16)AAT2);

    //
    /*
    WR1=WR0;
    WR3= WR2;
    WR2= WR4;
    ADC0value=(ADCBUF0*3.3/1024);
    WR0=refi-ADC0value; //Isensse error (x) //subtracting 2.5V (LT1999-20)
    WR4=A*WR0-B*WR1 +C*WR2 +D*WR3; //(y)
    PDC3=WR4;
*/

 //  if (x<2000){
 //       x++;
 //       IFS6bits.ADCP1IF = 0; //clear interrupt flag
 //       ADSTATbits.P1RDY = 0; //Clear ready bit
        
 //   }
 //  else{
    x=0;
     //WREG 5 x; WREG6 x1; WREG7 y1; WREG8 y2; WREG9 y
    WR6=WR5;
 //   WR8= WR7;
    WR7= WR9;
    ADC1value= (ADCBUF2*3.3/1024);
    //V_out sense
    //ADCBUF1: Vin_Sense
    //ADCBUF0: Current INTO the battery
    //ADCBUF3: Current from battery to converter

    WR5=refv- ADC1value; // 0.8 is (1024-775)=248 in dec; 2.4 is voltage divider
   
    WR9=WR7+kp*(WR5-WR6)+kp*TsTi*WR5; //(y)
//    refi=WR9;
    refi=WR9*(1923/3.3);
    if (refi>1200){
        PDC3=1200;
        WR9=1.8;
       
    }

    else {
            PDC3=refi;
             
           }

    
    LATFbits.LATF6 = 0;
    IFS6bits.ADCP1IF = 0; //clear interrupt flag   
    ADSTATbits.P1RDY = 0; //Clear ready bit

    battery_output_current_adc = ADCBUF3;
    battery_input_current_adc = ADCBUF0;
    battery_voltage_adc = ADCBUF1;
    network_voltage_adc = ADCBUF2;

    readings_updated = 1;

}
