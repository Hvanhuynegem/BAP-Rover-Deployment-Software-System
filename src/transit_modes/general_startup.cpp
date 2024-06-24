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


// Global variable for current task
TaskState generalStartupTask = TASK_SEND_TRANSIT_REQUEST;

// Function prototypes
void send_transit_mode_request_message(void);


void general_startup(void){
    // Initialize connection with the lander
    // Create an initialization message
    send_message_and_wait_for_ACK_3_times(MSG_TYPE_INIT, PAYLOAD_INIT, sizeof(PAYLOAD_INIT) - 1);

    // implement cooperative multi-tasking
    while(transit_state == GENERAL_STARTUP){
        switch(generalStartupTask){
            case TASK_SEND_TRANSIT_REQUEST:
                // Request the transit status from the lander
                send_transit_mode_request_message();
                generalStartupTask = TASK_CHECK_UMBILICAL;
                break;

            case TASK_CHECK_UMBILICAL:
                // is umbilical cord of the rover connected?
                initialize_umbilicalcord_pin_rover();
                bool status_umbilical_cord_rover = umbilicalcord_rover_connected();
                generalStartupTask = TASK_SEND_CONNECTION_STATUS;
                break;

            case TASK_SEND_CONNECTION_STATUS:
                // Send connection status message
                if (status_umbilical_cord_rover) {
                    send_message(MSG_TYPE_DATA, PAYLOAD_UMBILICAL_CONNECTED, sizeof(PAYLOAD_UMBILICAL_CONNECTED) - 1);
                } else {
                    send_message(MSG_TYPE_ERROR, PAYLOAD_UMBILICAL_NOT_CONNECTED, sizeof(PAYLOAD_UMBILICAL_NOT_CONNECTED) - 1);
                }
                generalStartupTask = TASK_SETUP_ROVER_CONNECTION;
                break;

            case TASK_SETUP_ROVER_CONNECTION:
                // Set up a connection with the rover
                /* TO BE IMPLEMENTED */
                generalStartupTask = TASK_RDS_CHECKUP;
                break;

            case TASK_RDS_CHECKUP:
                // Perform the RDS electronics checkup
                RDS_electronics_status_check();
                generalStartupTask = TASK_CLEAR;
                break;
            case TASK_CLEAR:
                generalStartupTask = TASK_SEND_TRANSIT_REQUEST;
                break;
            default:
                generalStartupTask = TASK_SEND_TRANSIT_REQUEST;
                break;
        }
        // Process received messages
        process_received_data();
    }
}


void send_transit_mode_request_message(void) {
    // Create a transit mode request message
    send_message(MSG_TYPE_REQUEST, PAYLOAD_TRANSIT_MODE, sizeof(PAYLOAD_TRANSIT_MODE) - 1);
}
