/*
 * deployment_mode.h
 *
 * This header file contains the function declarations and necessary includes for the deployment_mode.cpp file, which provides deployment mode functionalities for RDSS.
 *
 * Author: Henri Vanhuynegem
 * created: 24/06/2024
 * Last edited: 24/06/2024
 *
 */

#ifndef DEPLOYMENT_MODE_H
#define DEPLOYMENT_MODE_H

#include <msp430.h>
#include <stdint.h>
#include <string.h>

#include "lander_communication_lib/lander_communication.h"
#include "lander_communication_lib/lander_communication_protocol.h"
#include "lander_communication_lib/uart_communication.h"
#include "system_health_lib/temp_sensors.h"
#include "system_health_lib/umbilical_cord.h"
#include "system_health_lib/main_system_init.h"
#include "system_health_lib/bus_current_readout.h"
#include "system_health_lib/ECCS.h"
#include "transit_modes_lib/general_startup.h"

// Enumeration for task states
typedef enum {
    TASK_DEPLOY_SWITCH_OFF_HEATERS,
    TASK_DEPLOY_CHECK_SUPERCAP_FUNCTIONALITY,
    TASK_DEPLOY_CHECK_ALL_NEAS,
    TASK_DEPLOY_COMMUNICATE_DEPLOYMENT_TO_ROVER,
    TASK_DEPLOY_TURN_OFF_ROVER_POWER,
    TASK_DEPLOY_DISCONNECT_UMBILICAL,
    TASK_DEPLOY_ACTIVATE_NEAS,
    TASK_DEPLOY_DONE
} DeploymentTaskState;

extern DeploymentTaskState deploymentModeTask;

/*
 * deployment mode function for RDSS.
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  void
 */
void deployment_mode(void);


#endif // DEPLOYMENT_MODE_H
