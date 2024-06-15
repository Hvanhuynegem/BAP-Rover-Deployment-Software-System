/*
 * file "general_startup.cpp"
 *
 * general startup of RDSS
 *
 * Author: Henri Vanhuynegem
 * created: 15/06/2024
 * Last edited: 15/06/2024
 */

#include <msp430.h>
#include <stdint.h>
#include <string.h>

#include "transit_modes_lib/general_startup.h"



// Function prototypes
void send_transit_mode_request_message(void);
void send_message_and_wait_for_ACK(uint8_t msg_type, const uint8_t *payload, uint8_t length);

void general_startup(void){
    // Initialize connection with the lander
    // Create an initialization message
    uint8_t payload[] = "Initialization payload";
    send_message_and_wait_for_ACK(MSG_TYPE_INIT, payload, strlen((const char*)payload));

    // request the transit status from the lander
    send_transit_mode_request_message();

    // is umbilical cord of the rover connected?
    initialize_umbilicalcord_pin_rover();
    bool status_umbilical_cord_rover = read_umbilicalcord_pin_rover();

    // based on whether the umbilical cord is connected it sends either a correct connection or an error msg
    if(status_umbilical_cord_rover){
        uint8_t payload_correct[] = "umbilical cord connected";
        send_message(MSG_TYPE_DATA, payload_correct, strlen((const char*)payload_correct));
    }
    else{
        uint8_t payload_false[] = "umbilical cord not connected";
        send_message(MSG_TYPE_ERROR, payload_false, strlen((const char*)payload_false));
    }

    // set up a connection with the rover

    // perform the RDS electronics checkup


}


void send_transit_mode_request_message(void) {
    // Create a transit mode request message
    uint8_t payload[] = "TRANSIT_MODE";
    send_message(MSG_TYPE_REQUEST, payload, strlen((const char*)payload));
}
