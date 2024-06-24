/*
 * file "deployment_mode.cpp"
 *
 * deployment mode of RDSS
 *
 * Author: Henri Vanhuynegem
 * created: 24/06/2024
 * Last edited: 24/06/2024
 */

#include <msp430.h>
#include <stdint.h>
#include <string.h>

#include "transit_modes_lib/deployment_mode.h"

// Global variable for current task
DeploymentTaskState deploymentModeTask = TASK_DEPLOY_SWITCH_OFF_HEATERS;

void deployment_mode(void){
    // Implement cooperative multi-tasking
    while(transit_state == DEPLOYMENT){
        switch(deploymentModeTask){
            case TASK_DEPLOY_SWITCH_OFF_HEATERS:
                // Switch off the heaters
                MCU_heaterOn_low();
                MCU_heaterOff_low();
                deploymentModeTask = TASK_DEPLOY_CHECK_SUPERCAP_FUNCTIONALITY;
                break;

            case TASK_DEPLOY_CHECK_SUPERCAP_FUNCTIONALITY:
                // Check supercapacitor functionality
                check_supercap_functionality();
                deploymentModeTask = TASK_DEPLOY_CHECK_ALL_NEAS;
                break;

            case TASK_DEPLOY_CHECK_ALL_NEAS:
                // NEA checkup
                // Check the status of the 4 NEA's
                bool status_NEA_1 = read_NEAready_status(&P3IN, BIT1);
                bool status_NEA_2 = read_NEAready_status(&P3IN, BIT2);
                bool status_NEA_3 = read_NEAready_status(&P3IN, BIT3);
                bool status_NEA_4 = read_NEAready_status(&P4IN, BIT7);

                if (status_NEA_1 && status_NEA_2 && status_NEA_3 && status_NEA_4) {
                    // Send message that none of the NEA's is activated already
                    send_message(MSG_TYPE_DATA, PAYLOAD_ALL_NEA_READY, sizeof(PAYLOAD_ALL_NEA_READY) - 1);
                } else {
                    if (status_NEA_1) {
                        // Send message that NEA 1 is not activated yet
                        send_message(MSG_TYPE_DATA, PAYLOAD_NEA1_READY, sizeof(PAYLOAD_NEA1_READY) - 1);
                    } else {
                        // Send message that NEA 1 is already activated
                        send_message(MSG_TYPE_DATA, PAYLOAD_NEA1_NOT_READY, sizeof(PAYLOAD_NEA1_NOT_READY) - 1);
                    }
                    if (status_NEA_2) {
                        // Send message that NEA 2 is not activated yet
                        send_message(MSG_TYPE_DATA, PAYLOAD_NEA2_READY, sizeof(PAYLOAD_NEA2_READY) - 1);
                    } else {
                        // Send message that NEA 2 is already activated
                        send_message(MSG_TYPE_DATA, PAYLOAD_NEA2_NOT_READY, sizeof(PAYLOAD_NEA2_NOT_READY) - 1);
                    }
                    if (status_NEA_3) {
                        // Send message that NEA 3 is not activated yet
                        send_message(MSG_TYPE_DATA, PAYLOAD_NEA3_READY, sizeof(PAYLOAD_NEA3_READY) - 1);
                    } else {
                        // Send message that NEA 3 is already activated
                        send_message(MSG_TYPE_DATA, PAYLOAD_NEA3_NOT_READY, sizeof(PAYLOAD_NEA3_NOT_READY) - 1);
                    }
                    if (status_NEA_4) {
                        // Send message that NEA 4 is not activated yet
                        send_message(MSG_TYPE_DATA, PAYLOAD_NEA4_READY, sizeof(PAYLOAD_NEA4_READY) - 1);
                    } else {
                        // Send message that NEA 4 is already activated
                        send_message(MSG_TYPE_DATA, PAYLOAD_NEA4_NOT_READY, sizeof(PAYLOAD_NEA4_NOT_READY) - 1);
                    }
                }
                deploymentModeTask = TASK_DEPLOY_COMMUNICATE_DEPLOYMENT_TO_ROVER;
                break;

            case TASK_DEPLOY_COMMUNICATE_DEPLOYMENT_TO_ROVER:
                // Communicate deployment status to rover
                deploymentModeTask = TASK_DEPLOY_TURN_OFF_ROVER_POWER;
                break;

            case TASK_DEPLOY_TURN_OFF_ROVER_POWER:
                // Turn off rover power
                switch_off_bus_flag_pin();
                send_message(MSG_TYPE_DATA, PAYLOAD_POWER_ROVER_OFF, sizeof(PAYLOAD_POWER_ROVER_OFF) - 1);
                deploymentModeTask = TASK_DEPLOY_DISCONNECT_UMBILICAL;
                break;

            case TASK_DEPLOY_DISCONNECT_UMBILICAL:
                // Disconnect umbilical cord
                detach_umbilicalcord();
                deploymentModeTask = TASK_DEPLOY_ACTIVATE_NEAS;
                break;

            case TASK_DEPLOY_ACTIVATE_NEAS:
                // Activate NEAs
                activate_NEAs();
                deploymentModeTask = TASK_DEPLOY_DONE;
                break;

            case TASK_DEPLOY_DONE:
                // Deployment done
                // Optionally, set a flag or perform an action indicating deployment is complete
                send_message(MSG_TYPE_DATA, PAYLOAD_DEPLOYMENT_COMPLETE, sizeof(PAYLOAD_DEPLOYMENT_COMPLETE) - 1);
                break;

            default:
                deploymentModeTask = TASK_DEPLOY_DONE;
                break;
        }
        // Process received messages
        process_received_data();
    }
}
