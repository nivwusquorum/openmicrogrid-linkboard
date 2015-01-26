#include "SensorManager.h"
#include "p33FJ64GS406.h"
#include <libq.h>
#include <dsp.h>


float getCurrentIntoBattery(){
    float sensorVal = (ADCBUF0*3.3/1024-0.08)*(float)10;
    //sensorVal = 7.5;
    return sensorVal;
}

float getCurrentOutOfBattery(){
    float sensorVal = (ADCBUF3*3.3/1024-0.08)*(float)10;
    //sensorVal = 7.5;
    return sensorVal;
}

float getOutputVoltage(){
    float sensorVal = ADCBUF2*3.3/1024*(float)10;
    //sensorVal = 7.5
    return sensorVal;
}

float getInputVoltage(){
    float sensorVal  = ADCBUF1*3.3/1024*(float)5.2;
    //sensorVal = 93.1;
    return sensorVal;
}