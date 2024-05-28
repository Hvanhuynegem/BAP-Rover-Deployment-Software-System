/*
 * Main file to test lander communication
 *
 * Author: Henri Vanhuynegem
 * Date: 23-05-2024
 */
#include "communication/landerCommProtocol.h"
#include "communication/landerComm.h"
#include <msp430.h>
#include <stdint.h>
#include <string.h>


// Function prototypes
void initialize_system(void);
void send_init_message(void);

int main(void) {
    // Stop watchdog timer
    WDTCTL = WDTPW | WDTHOLD;

    // Initialize system (clock, UART)
    initialize_system();



    // Send initialization message
    send_init_message();

    // Enter low-power mode and enable global interrupts
    __bis_SR_register(GIE);

    // Main loop (not used in this example)
    while (1) {
        // Do nothing, wait for interrupts
    }
}

void initialize_system(void) {
    // Initialize clock
    initialize_clock();

    // Initialize UART
    initialize_UART_A1();
}

void send_init_message(void) {
    // Create an initialization message
    uint8_t payload[] = "Initialization payload";
    Message init_msg = create_message(MSG_TYPE_INIT, payload, strlen((const char*)payload));

    // Send the initialization message
    send_message(&init_msg);
}

