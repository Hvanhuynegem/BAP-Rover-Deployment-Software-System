#include <msp430.h>

// Delay function for simplicity
void delay(void) {
    volatile unsigned int i;  // Declare as volatile to prevent optimization
    for (i = 50000; i > 0; i--); // Arbitrary delay
}

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    // Disable the GPIO power-on default high-impedance mode
    // to activate previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    // Set up pin P1.0 as output
    P1DIR |= BIT0;             // Set P1.0 to output direction

    // Main loop
    while(1) {
        P1OUT ^= BIT0;         // Toggle P1.0 using exclusive-OR
        delay();               // Wait
    }
}
