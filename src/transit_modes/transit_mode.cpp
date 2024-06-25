/*
 * file "transit_mode.cpp"
 *
 * transit mode of RDSS
 *
 * Author: Henri Vanhuynegem
 * created: 24/06/2024
 * Last edited: 24/06/2024
 */

#include <msp430.h>
#include <stdint.h>
#include <string.h>

#include "transit_modes_lib/transit_mode.h"


// Global variable for current task
TaskState transitModeTask = TASK_SETUP_ROVER_CONNECTION;

void transit_mode(void){
    transitModeTask = TASK_SETUP_ROVER_CONNECTION;
    // implement cooperative multi-tasking
    while(transit_state == TRANSIT){
        switch(transitModeTask){
            case TASK_SETUP_ROVER_CONNECTION:
                // Set up a connection with the rover
                /* TO BE IMPLEMENTED */
                transitModeTask = TASK_RDS_CHECKUP;
                break;

            case TASK_RDS_CHECKUP:
                // Perform the RDS electronics checkup
                RDS_electronics_status_check();
                transitModeTask = TASK_CLEAR;
                break;
            case TASK_CLEAR:
                transitModeTask = TASK_SETUP_ROVER_CONNECTION;
                break;
            default:
                transitModeTask = TASK_SETUP_ROVER_CONNECTION;
                break;
        }
        // Process received messages
        process_received_data();
    }
}
