/*
 * Main file to test lander communication
 *
 * Author: Henri Vanhuynegem
 * created: 23/05/2024
 * Last edited: 3/06/2024
 */

#include <msp430.h>
#include <stdint.h>
#include <string.h>
#include <transit_modes_lib/general_startup.h>

#include "lander_communication_lib/lander_communication.h"
#include "lander_communication_lib/lander_communication_protocol.h"
#include "lander_communication_lib/uart_communication.h"

int main(void) {
    // Stop watchdog timer
    WDTCTL = WDTPW | WDTHOLD;

    // digital clk
    setup_SMCLK();
    // Initialise UART
    uart_configure();

    // Pins start in high-impedance mode on wake-up, this enables them again
    // and activates our port configurations
    PM5CTL0 &= ~LOCKLPM5;

    // enable global interrupts
    __bis_SR_register(GIE);

    // Main loop (not used in this example)
    while (1) {
        general_startup();
    }
}



