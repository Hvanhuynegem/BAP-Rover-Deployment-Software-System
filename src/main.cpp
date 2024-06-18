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
        switch (transit_state){
        case GENERAL_STARTUP:
            // process RX buffer
            process_received_data();
            uint8_t payload0[] = "GENERAL_STARTUP";
            send_message(MSG_TYPE_RESPONSE, payload0, strlen((const char*)payload0));
            general_startup();
            break;
        case LAUNCH_INTEGRATION:
            uint8_t payload1[] = "LAUNCH_INTEGRATION";
            send_message(MSG_TYPE_RESPONSE, payload1, strlen((const char*)payload1));
            break;
        case TRANSIT:
            uint8_t payload2[] = "TRANSIT";
            send_message(MSG_TYPE_RESPONSE, payload2, strlen((const char*)payload2));
            break;
        case PRE_DEPLOYMENT:
            uint8_t payload3[] = "PRE_DEPLOYMENT";
            send_message(MSG_TYPE_RESPONSE, payload3, strlen((const char*)payload3));
            break;
        case DEPLOYMENT:
            uint8_t payload4[] = "DEPLOYMENT";
            send_message(MSG_TYPE_RESPONSE, payload4, strlen((const char*)payload4));
            break;
        default:
            // process RX buffer
            process_received_data();
            general_startup();
            break;
        }
    }
}


