/*
 * file "launch_mode.cpp"
 *
 * launch mode of RDSS
 *
 * Author: Henri Vanhuynegem
 * created: 24/06/2024
 * Last edited: 24/06/2024
 */

#include <msp430.h>
#include <stdint.h>
#include <string.h>

#include "transit_modes_lib/launch_mode.h"


// Global variable for current task
TaskState launchModeTask = TASK_CHECK_UMBILICAL;


void launch_mode(void){
    launchModeTask = TASK_CHECK_UMBILICAL;
    // Initialize connection with the lander
    // Create an initialization message
    send_message_and_wait_for_ACK_3_times(MSG_TYPE_INIT, PAYLOAD_INIT, sizeof(PAYLOAD_INIT) - 1);

    // implement cooperative multi-tasking
    while(transit_state == LAUNCH_INTEGRATION){
        switch(launchModeTask){
            case TASK_CHECK_UMBILICAL:
                // is umbilical cord of the rover connected?
                initialize_umbilicalcord_pin_rover();
                bool status_umbilical_cord_rover = umbilicalcord_rover_connected();
                launchModeTask = TASK_SEND_CONNECTION_STATUS;
                break;

            case TASK_SEND_CONNECTION_STATUS:
                // Send connection status message
                if (status_umbilical_cord_rover) {
                    send_message(MSG_TYPE_DATA, PAYLOAD_UMBILICAL_CONNECTED, sizeof(PAYLOAD_UMBILICAL_CONNECTED) - 1);
                } else {
                    send_message(MSG_TYPE_ERROR, PAYLOAD_UMBILICAL_NOT_CONNECTED, sizeof(PAYLOAD_UMBILICAL_NOT_CONNECTED) - 1);
                }
                launchModeTask = TASK_SETUP_ROVER_CONNECTION;
                break;

            case TASK_SETUP_ROVER_CONNECTION:
                // Set up a connection with the rover
                /* TO BE IMPLEMENTED */
                launchModeTask = TASK_RDS_CHECKUP;
                break;

            case TASK_RDS_CHECKUP:
                // Perform the RDS electronics checkup
                RDS_electronics_status_check();
                launchModeTask = TASK_CLEAR;
                break;
            case TASK_CLEAR:
                launchModeTask = TASK_CHECK_UMBILICAL;
                break;
            default:
                launchModeTask = TASK_CHECK_UMBILICAL;
                break;
        }
        // Process received messages
        process_received_data();
    }
}
