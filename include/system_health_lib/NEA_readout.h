/*
 * NEA_readout.h
 *
 * This header file contains the function declarations and necessary includes for the NEA_readout.cpp file, which provides functionalities for reading the NEA ready flags from pins.
 *
 * Author: Diederik Aris
 * created: 28/05/2024
 * Last edited: 15/06/2024
 *
 */

#ifndef NEA_READOUT_H
#define NEA_READOUT_H

#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

/*
 * Initializes all 4 NEA's.
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  void
 */
void initialize_all_nea_pins(void);

/*
 * Reads the status of the NEA ready bit.
 *
 * Parameters:
 *  volatile uint8_t *port_in : pointer to the input port register
 *  uint8_t pin : the pin to check the status of
 *
 * Returns:
 *  bool : the status of the specified pin (true if high, false if low)
 */
bool read_NEAready_status(volatile uint8_t *port_in, uint8_t pin);

#endif // NEA_READOUT_H
