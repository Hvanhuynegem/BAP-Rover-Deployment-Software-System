/*
 * file "pre_deployment_mode.cpp"
 *
 * pre-deployment mode of RDSS
 *
 * Author: Henri Vanhuynegem
 * created: 24/06/2024
 * Last edited: 24/06/2024
 */

#include <msp430.h>
#include <stdint.h>
#include <string.h>

#include "transit_modes_lib/pre_deployment_mode.h"


// Global variable for current task
TaskState preDeploymentModeTask = TASK_SETUP_ROVER_CONNECTION;

void pre_deployment_mode(void){
    // implement cooperative multi-tasking
    while(transit_state == PRE_DEPLOYMENT){
        switch(preDeploymentModeTask){
            case TASK_SETUP_ROVER_CONNECTION:
                // Set up a connection with the rover
                /* TO BE IMPLEMENTED */
                preDeploymentModeTask = TASK_RDS_CHECKUP;
                break;

            case TASK_RDS_CHECKUP:
                // Perform the RDS electronics checkup
                RDS_electronics_status_check();
                preDeploymentModeTask = TASK_CLEAR;
                break;
            case TASK_CLEAR:
                preDeploymentModeTask = TASK_SETUP_ROVER_CONNECTION;
                break;
            default:
                preDeploymentModeTask = TASK_SETUP_ROVER_CONNECTION;
                break;
        }
        // Process received messages
        process_received_data();
    }
}
