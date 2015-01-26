#include "ReadingModel.h"

#include <math.h>
#include <stdlib.h>

double battery_output_current;
double battery_input_current;
double battery_voltage;
double network_voltage;

//////////////////////////////////////////

void on_network_voltage_reading(double reading){
    network_voltage = ADC_FORGET*(reading) + (1-ADC_FORGET)*network_voltage;
}

void on_battery_voltage_reading(double reading){
    battery_voltage = ADC_FORGET*(reading) + (1-ADC_FORGET)*battery_voltage;
}

void on_battery_input_current_reading(double reading){
    battery_input_current = ADC_FORGET*(reading) + (1-ADC_FORGET)*battery_input_current;
}
void on_battery_output_current_reading(double reading){
    battery_output_current = ADC_FORGET*(reading) + (1-ADC_FORGET)*battery_output_current;
}

////////////////////////////////////////////////

double get_network_voltage() {
    return network_voltage;
}

double get_battery_voltage() {
    return battery_voltage;
}

double get_battery_input_current() {
    return battery_input_current;
}

double get_battery_output_current() {
    return battery_output_current;
}