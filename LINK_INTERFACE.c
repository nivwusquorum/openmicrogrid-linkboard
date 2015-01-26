#include "LINK_INTERFACE.h"

#include "ReadingModel.h"
#include "SPI_DRIVER.h"
#include "LinkBoardMessageConsts.h"

int next_index = 0;
uint8_t buffer[4];

void put_float_in_buffer(float f) {
    uint8_t* ptr = (uint8_t*)&f;
    int i;
    for (i=0; i<4; ++i) {
        buffer[i] = *ptr;
        ptr += 1;
    }
    next_index = 0;
}


uint8_t receiveMessageComm(uint8_t msg){
    switch(msg){
        case SPI_REQUEST_DATA:
            if (next_index < 4) {
                return buffer[next_index++];
            } else {
                // should never happen.
                return 0;
            }
            break;

        //Following cases are sensor values
        case LINK_BATTERY_INPUT_CURRENT:
            put_float_in_buffer(get_battery_input_current());
            return 0;
            //sendPollFloatToComm(9.5);
            break;
        case LINK_BATTERY_OUTPUT_CURRENT:
            put_float_in_buffer(get_battery_output_current());
            return 0;
            break;
        case LINK_NETWORK_VOLTAGE:
            put_float_in_buffer(get_network_voltage());
            return 0;
            break;
        case LINK_BATTERY_VOLTAGE:
            put_float_in_buffer(get_battery_voltage());
            return 0;
            break;
        default:
            return 123;
           break;
    }

}















