/*
 * umbilical_cord.cpp file
 *
 * This file includes the functionalities for checking whether the umbilical cord is connected.
 *
 * Author: Diederik Aris
 * created: 28/05/2024
 * Last edited: 15/06/2024
 *
 */

#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

#include "system_health_lib/umbilical_cord.h"


// Function to initialize P2.2 as an input pin
void initialize_umbilicalcord_pin_rover(void) {
    // Configure GPIO
    P2DIR &= ~BIT2;                         // Set P2.2 as input (Umbilical_cord_status_bit)
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
}

// Function to read the status of P2.2 (umbilical cord rover)
bool umbilicalcord_rover_connected(void) {
    return (P2IN & BIT2) != 0;              // Return the status of P2.2
}


// Function to initialize P4.6 for detachment of umbilical cord
void initialize_umbilicalcord_detach_pin(void){
    // configure GPIO
    P4DIR |= BIT6;                         // Set P4.6 as output (DetachUmb)
    P4OUT &= ~BIT6;
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
}

// Function to set P4.6 high for detachment of umbilical cord
void detach_umbilicalcord(void){
    P4OUT |= BIT6;                          // Set P4.6 high
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
    int x = 0;
    while(umbilicalcord_rover_connected()){
        if(x == 3){
            return;
        }
        if (umbilicalcord_rover_connected()) {
            send_message(MSG_TYPE_DATA, PAYLOAD_UMBILICAL_CONNECTED, sizeof(PAYLOAD_UMBILICAL_CONNECTED) - 1);
        } else {
            send_message(MSG_TYPE_ERROR, PAYLOAD_UMBILICAL_NOT_CONNECTED, sizeof(PAYLOAD_UMBILICAL_NOT_CONNECTED) - 1);
        }
        x++;
    }
}
