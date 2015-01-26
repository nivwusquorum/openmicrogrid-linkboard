#include "init.h"

#include "p33FJ64GS406.h"

void init_clock()
{
    /* Configure Oscillator to operate the device at 40Mhz
    Fosc= Fin*M/(N1*N2), Fcy=Fosc/2
    Fosc= 7.37*(43)/(2*2)=80Mhz for Fosc, Fcy = 40Mhz */
    /* Configure PLL prescaler, PLL postscaler, PLL divisor */
    PLLFBD=41; 				/* M = PLLFBD + 2 */
    CLKDIVbits.PLLPOST=0;   /* N1 = 2 */
    CLKDIVbits.PLLPRE=0;    /* N2 = 2 */

    __builtin_write_OSCCONH(0x01);			/* New Oscillator FRC w/ PLL */
    __builtin_write_OSCCONL(0x01);  		/* Enable Switch */

    while(OSCCONbits.COSC != 0b001);		/* Wait for new Oscillator to become FRC w/ PLL */
    while(OSCCONbits.LOCK != 1);			/* Wait for Pll to Lock */

    /* Now setup the ADC and PWM clock for 120MHz
    ((FRC * 16) / APSTSCLR ) = (7.37 * 16) / 1 = ~ 120MHz*/

    ACLKCONbits.FRCSEL = 1;					/* FRC provides input for Auxiliary PLL (x16) */
    ACLKCONbits.SELACLK = 1;				/* Auxiliary Oscillator provides clock source for PWM & ADC */
    ACLKCONbits.APSTSCLR = 7;				/* Divide Auxiliary clock by 1 */
    ACLKCONbits.ENAPLL = 1;					/* Enable Auxiliary PLL */

    while(ACLKCONbits.APLLCK != 1);			/* Wait for Auxiliary PLL to Lock */
}

void init_ADC ()
{
    ADCONbits.FORM    = 0;       // Integer data format
    ADCONbits.EIE     = 0;       // Early Interrupt disabled
    ADCONbits.ORDER   = 0;       // Convert even channel first
    ADCONbits.SEQSAMP = 0;       // Select simultaneous sampling
    ADCONbits.ADCS    = 5;       // ADC clock = FADC/6 = 120MHz / 6 = 20MHz,
				 // 12*Tad = 1.6 MSPS

    //IFS6bits.ADCP0IF  = 0;	 // Clear ADC interrupt flag
    //IPC27bits.ADCP0IP = 5;	 // Set ADC interrupt priority
    //IEC6bits.ADCP0IE  = 1;	 // Enable the ADC Pair 0 interrupt

    IFS6bits.ADCP1IF  = 0;	 // Clear ADC interrupt flag pair 1
    IPC27bits.ADCP1IP = 3;	 // Set ADC interrupt priority pair 1
    IEC6bits.ADCP1IE  = 1;       // Enable the ADC pair 1 interrupt

    ADPCFGbits.PCFG0 = 0; 	 // Current Measurement for Boost
    ADPCFGbits.PCFG1 = 0; 	 // Input Voltage Measurement for Boost
    ADPCFGbits.PCFG2 = 0; 	 // Output Voltage Measurement for Boost
    ADPCFGbits.PCFG3 = 0;

    //ADSTATbits.P0RDY   = 0; 	 // Clear Pair 1 data ready bit
    //ADCPC0bits.IRQEN0  = 1;      // Enable ADC Interrupt for Boost control loop
    ADCPC0bits.TRGSRC0 = 6; 	 // ADC Pair 1 triggered by PWM3

    ADSTATbits.P1RDY   = 0; 	 // Clear Pair 1 data ready bit
    ADCPC0bits.IRQEN1  = 1;      // Enable ADC Interrupt for Boost control loop
    ADCPC0bits.TRGSRC1 = 6; 	 // ADC Pair 1 triggered by PWM3

    ADCONbits.ADON = 1;		 // Enable the ADC Module early for ADC Settling Time
}

void init_PWM()
{
    PTPER = 1923;             				/* PTPER = 1/(500kHz*1.04ns) = 1923, where 500 khz is freq
							of the PWM  and 1.04ns is PWM resolution. */
    /* ~~~~~~~~~~~~~~~~~~~~~~ PWM1 Configuration ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
    IOCON3bits.PENH = 1;   					/* PWM1H is controlled by PWM module */
    IOCON3bits.PENL = 1;   					/* PWM1L is controlled by PWM module */
    IOCON3bits.PMOD = 0;   					/* Select Complementary Output PWM mode */
    IOCON3bits.SWAP = 1;

    PDC3 = 900;                 			/* PDC = (PTPER / DutyCycle)  */
    DTR3    = 30;               			/* Deadtime = (80ns / 1.04ns) where 65ns is desired deadtime */
    ALTDTR3 = 30;    						/* ALTDeadtime = (65ns / 1.04ns) where 65ns is desired deadtime */
    PHASE3 = 0;     			            /* No phase shift */

    TRGCON3bits.TRGDIV  =15;       // Trigger interrupt generated once every 15 PWM cycle
    TRGCON3bits.TRGSTRT = 1;      // Trigger generated after waiting 0 PWM cycles

    PTCONbits.PTEN = 1;			   			/* Enable the PWM Module */
}
