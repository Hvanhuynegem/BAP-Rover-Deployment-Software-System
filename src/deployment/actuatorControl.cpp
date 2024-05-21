#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

// Function to initialize P3.0 as an input pin
void initialize_umbilicalcord_pin_rover(void) {
    // Configure GPIO
    P3DIR &= ~BIT0;                         // Set P3.0 as input (Umbilical_cord_status_bit)
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
}

// Function to read the status of P3.0
bool read_umbilicalcord_pin_rover(void) {
    return (P3IN & BIT0) != 0;              // Return the status of P3.0
}
