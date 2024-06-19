/*
 * NEA_readout.cpp
 *
 * This file includes the functions needed to read the NEA ready flags from pins
 *
 *
 * Author: Diederik Aris
 * created: 28/05/2024
 * Last edited: 15/06/2024
 *
 */

#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>
#include <system_health_lib/NEA_readout.h>

// Function to initialize all 4 NEA's
void initialize_all_nea_pins(void) {

    // NEA 1
    P3DIR &= ~BIT1;                         // Set P3.1 as input pin (NEA Ready for NEA 1)
    P1DIR |= BIT0;                          // Set P1.0 as output pin (NEA Flag for NEA 1)
    P1OUT &= ~BIT0;                         // Initialize P1.0 to low (don't activate NEA 1)

    // NEA 2
    P3DIR &= ~BIT2;                         // Set P3.2 as input pin (NEA Ready for NEA 2)
    P1DIR |= BIT1;                          // Set P1.1 as output pin (NEA Flag for NEA 2)
    P1OUT &= ~BIT1;                         // Initialize P1.1 to low (don't activate NEA 2)

    // NEA 3
    P3DIR &= ~BIT3;                         // Set P3.3 as input pin (NEA Ready for NEA 3)
    P1DIR |= BIT2;                          // Set P1.2 as output pin (NEA Flag for NEA 3)
    P1OUT &= ~BIT2;                         // Initialize P1.2 to low (don't activate NEA 3)

    // NEA 4
    P4DIR &= ~BIT7;                         // Set P4.7 as input pin (NEA Ready for NEA 3)
    P3DIR |= BIT0;                          // Set P3.0 as output pin (NEA Flag for NEA 3)
    P3OUT &= ~BIT0;                         // Initialize P3.0 to low (don't activate NEA 3)

    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
}




// Function to read the status of the NEA ready bit
bool read_NEAready_status(volatile uint8_t *port_in, uint8_t pin) {
    return (*port_in & pin) != 0;  // Return the status of the specified pin
}
