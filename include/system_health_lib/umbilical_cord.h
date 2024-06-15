/*
 * umbilical_cord.h
 *
 * This header file contains the function declarations and necessary includes for the umbilical_cord.cpp file, which provides functionalities for checking whether the umbilical cord is connected.
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

/*
 * Initializes P3.0 as an input pin.
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  void
 */
void initialize_umbilicalcord_pin_rover(void);

/*
 * Reads the status of P3.0 (umbilical cord rover).
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  bool : the status of P3.0 (true if high, false if low)
 */
bool read_umbilicalcord_pin_rover(void);

#endif // UMBILICAL_CORD_H
