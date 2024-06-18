/*
 * umbilical_cord.cpp file
 *
 * This file includes the functionalities for checking the bus current.
 *
 * Author: Henri Vanhuynegem
 * created: 18/06/2024
 * Last edited: 18/06/2024
 *
 */

#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

#include "system_health_lib/bus_current_readout.h"


// Function to initialize pin 4.3 as an input pin
void initialize_bus_current_sense_pin(void) {
    // Configure GPIO
    P4DIR &= ~BIT3;                         // Set pin 4.3 as input (bus current sensing pin)
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
}

// Function to initialize pin 4.2 as an output pin
void initialize_bus_flag_pin(void) {
    // Configure GPIO
    P4DIR |= BIT2;                         // Set pin 4.2 as output (bus flag pin)
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
}
