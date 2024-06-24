/*
 * transit_mode.h
 *
 * This header file contains the function declarations and necessary includes for the transit_mode.cpp file, which provides transit mode functionalities for RDSS.
 *
 * Author: Henri Vanhuynegem
 * created: 24/06/2024
 * Last edited: 24/06/2024
 *
 */

#ifndef TRANSIT_MODE_H
#define TRANSIT_MODE_H

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
 * transit mode function for RDSS.
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  void
 */
void transit_mode(void);


#endif // TRANSIT_MODE_H
