#include "p33FJ64GS406.h"

#include <libq.h>
#include <math.h>
#include "SPI_DRIVER.h"
#include "init.h"

extern unsigned int TimerInterruptCount;

/* Configuration Bit Settings */
_FBS(BWRP_WRPROTECT_OFF);
_FGS(GSS_OFF&GCP_OFF&GWRP_OFF);
_FOSCSEL(FNOSC_FRC)
_FOSC(FCKSM_CSECMD & OSCIOFNC_ON & POSCMD_NONE ) //it was CSECMDandON
_FWDT(FWDTEN_OFF)
_FPOR(FPWRT_PWR128 )
_FICD(ICS_PGD2 & JTAGEN_OFF)



int main()
{

        init_clock();
	init_PWM();
        init_ADC();

        initSPI();

        while(1){}
}

