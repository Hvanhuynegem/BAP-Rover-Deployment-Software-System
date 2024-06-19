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
    send_message_and_wait_for_ACK(MSG_TYPE_INIT, PAYLOAD_INIT, sizeof(PAYLOAD_INIT) - 1);

    // request the transit status from the lander
    send_transit_mode_request_message();

    // process RX buffer
    process_received_data();

    // is umbilical cord of the rover connected?
    initialize_umbilicalcord_pin_rover();
    bool status_umbilical_cord_rover = umbilicalcord_rover_connected();

    // process RX buffer
    process_received_data();

    // based on whether the umbilical cord is connected it sends either a correct connection or an error msg
    if(status_umbilical_cord_rover){
        send_message(MSG_TYPE_DATA, PAYLOAD_UMBILICAL_CONNECTED, sizeof(PAYLOAD_UMBILICAL_CONNECTED) - 1);
    }
    else{
        send_message(MSG_TYPE_ERROR, PAYLOAD_UMBILICAL_NOT_CONNECTED, sizeof(PAYLOAD_UMBILICAL_NOT_CONNECTED) - 1);
    }

    // process RX buffer
    process_received_data();

    // set up a connection with the rover
    /* TO BE IMPLEMENTED*/

    // perform the RDS electronics checkup
    if(transit_state == GENERAL_STARTUP){
        RDS_electronics_status_check();
    }

}


void send_transit_mode_request_message(void) {
    // Create a transit mode request message
    send_message(MSG_TYPE_REQUEST, PAYLOAD_TRANSIT_MODE, sizeof(PAYLOAD_TRANSIT_MODE) - 1);
}
