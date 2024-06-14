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

#include "lander_communication_lib/lander_communication.h"
#include "lander_communication_lib/lander_communication_protocol.h"
#include "lander_communication_lib/uart_communication.h"

// Function prototypes
void initialize_system(void);
void send_init_message(void);
void process_received_data(void);


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
        process_received_data();
//        send_init_message();
//        __delay_cycles(3000);
        // Do nothing, wait for interrupts
    }
}


void send_init_message(void) {
    // Create an initialization message
    uint8_t payload[] = "Initialization payload";
    send_message(MSG_TYPE_INIT, payload, strlen((const char*)payload));
}

