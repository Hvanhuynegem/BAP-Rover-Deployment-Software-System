/*
 * umbilical_cord.h
 *
 * This header file contains the function declarations and necessary includes for the umbilical_cord.cpp file, which provides functionalities for checking whether the umbilical cord is connected and for detaching it.
 *
 * Author: Diederik Aris
 * created: 28/05/2024
 * Last edited: 15/06/2024
 *
 */

#ifndef UMBILICAL_CORD_H
#define UMBILICAL_CORD_H

#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

#include "lander_communication_lib/lander_communication.h"
#include "lander_communication_lib/payload_messages.h"

/*
 * Initializes P2.2 as an input pin.
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  void
 */
void initialize_umbilicalcord_pin_rover(void);

/*
 * Reads the status of P2.2 (umbilical cord rover).
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  bool : the status of P2.2 (true if high, false if low)
 */
bool umbilicalcord_rover_connected(void);

/*
 * Initializes P4.6 for detachment of umbilical cord.
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  void
 */
void initialize_umbilicalcord_detach_pin(void);

/*
 * Sets P4.6 high for detachment of umbilical cord.
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  void
 */
void detach_umbilicalcord(void);

#endif // UMBILICAL_CORD_H
