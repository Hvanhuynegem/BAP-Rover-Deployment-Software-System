
// transmitter code
#include "communication/landerComm.h"
#include <msp430.h>
#include <stdint.h>

#define TEST_STRING "TEST1234"
#define BAUD_RATE 9600

void delay(void) {
    volatile unsigned int i;  // Declare as volatile to prevent optimization
    for (i = 50000; i > 0; i--); // Arbitrary delay
}

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;  // Stop watchdog timer

    // Disable the GPIO power-on default high-impedance mode
    // to activate previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    initialize_UART(BAUD_RATE);

    // Set up pin P1.0 as output
    P1DIR |= BIT0;             // Set P1.0 to output direction
    P1OUT ^= BIT0;         // Toggle P1.0 using exclusive-OR

    // Main loop
    while (1) {
        slip_encode((uint8_t *)TEST_STRING, sizeof(TEST_STRING) - 1);
        delay();  // Delay to allow the receiver to process
    }
}

/*

// receiver code
#include "communication/landerComm.h"
#include <msp430.h>
#include <stdint.h>
#include <string.h>  // Include string.h for memcmp

#define BAUD_RATE 9600
#define LED_PIN BIT0

void initialize_ports(void) {
    P1DIR |= LED_PIN;  // Set P1.0 as output (LED)
    P1OUT &= ~LED_PIN; // Turn off LED
}

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;  // Stop watchdog timer
    initialize_ports();
    initialize_UART(BAUD_RATE);

    uint8_t rx_buffer[256];
    uint16_t received_length = 0;

    // Main loop
    while (1) {
        slip_decode(rx_buffer, &received_length);
        if (received_length == 8 && memcmp(rx_buffer, "TEST1234", 8) == 0) {
            P1OUT |= LED_PIN;  // Turn on LED if received "TEST1234"
        } else {
            P1OUT &= ~LED_PIN; // Turn off LED otherwise
        }
    }
}
*/

