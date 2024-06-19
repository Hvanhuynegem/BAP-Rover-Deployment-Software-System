/*
 * general_startup.h
 *
 * This header file contains the function declarations and necessary includes for the general_startup.cpp file, which provides general startup functionalities for RDSS.
 *
 * Author: Henri Vanhuynegem
 * created: 15/06/2024
 * Last edited: 15/06/2024
 *
 */

#ifndef GENERAL_STARTUP_H
#define GENERAL_STARTUP_H

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


/*
 * General startup function for RDSS.
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  void
 */
void general_startup(void);

/*
 * Sends a transit mode request message.
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  void
 */
void send_transit_mode_request_message(void);

#endif // GENERAL_STARTUP_H
