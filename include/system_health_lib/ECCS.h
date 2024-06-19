/*
 * ECCS.h
 *
 * This header file contains the function declarations and necessary includes for the ECCS.cpp file, which provides functionalities for doing the Electronic Components Control System check.
 *
 * Author: Henri Vanhuynegem
 * created: 19/06/2024
 * Last edited: 19/06/2024
 *
 */

#ifndef ECCS_H
#define ECCS_H

#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

// Include necessary header files
#include "system_health_lib/umbilical_cord.h"
#include "system_health_lib/main_system_init.h"
#include "system_health_lib/bus_current_readout.h"
#include "system_health_lib/supercap_readout.h"
#include "system_health_lib/NEA_readout.h"
#include "system_health_lib/temp_sensors.h"
#include "system_health_lib/heat_resistor_control.h"
#include "lander_communication_lib/lander_communication.h"
#include "lander_communication_lib/payload_messages.h"

/*
 * Initializes all the electronic pins.
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  void
 */
void initialize_all_electronic_pins(void);

/*
 * Performs the RDS electronics status check.
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  void
 */
void RDS_electronics_status_check(void);

#endif // ECCS_H
