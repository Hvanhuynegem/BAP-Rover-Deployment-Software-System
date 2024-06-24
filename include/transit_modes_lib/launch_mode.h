/*
 * launch_mode.h
 *
 * This header file contains the function declarations and necessary includes for the launch_mode.cpp file, which provides launch integration functionalities for RDSS.
 *
 * Author: Henri Vanhuynegem
 * created: 24/06/2024
 * Last edited: 24/06/2024
 *
 */

#ifndef LAUNCH_MODE_H
#define LAUNCH_MODE_H

#include <msp430.h>
#include <stdint.h>
#include <string.h>

#include "lander_communication_lib/lander_communication.h"
#include "lander_communication_lib/lander_communication_protocol.h"
#include "lander_communication_lib/uart_communication.h"
#include "system_health_lib/temp_sensors.h"
#include "system_health_lib/umbilical_cord.h"
#include "system_health_lib/main_system_init.h"
#include "system_health_lib/ECCS.h"
#include "transit_modes_lib/general_startup.h"


/*
 * launch mode function for RDSS.
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  void
 */
void launch_mode(void);


#endif // LAUNCH_MODE_H
