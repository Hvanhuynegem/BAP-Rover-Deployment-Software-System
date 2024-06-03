/*
 * Main file to test lander communication
 *
 * Author: Henri Vanhuynegem
 * created: 23/05/2024
 * Last edited: 3/06/2024
 */

#include "communication/landerCommProtocol.h"
#include "communication/landerComm.h"
#include <msp430.h>
#include <stdint.h>
#include <string.h>

// Function prototypes
void initialize_system(void);
void send_init_message(void);
void process_received_data(void);

int main(void) {
    // Stop watchdog timer
    WDTCTL = WDTPW | WDTHOLD;

    // Initialize UART
    initialize_UART_A1_115200();

    // Enter low-power mode and enable global interrupts
    __bis_SR_register(GIE);

    // Main loop (not used in this example)
    while (1) {
//        process_received_data();

        if(TX_start == TX_end){
            //delay
            __delay_cycles(1000000); // 1 second == 1000000
            // Send initialization message
            send_init_message();
        }
        // Do nothing, wait for interrupts
    }
}


void send_init_message(void) {
    // Create an initialization message
    uint8_t payload[] = "Initialization payload";
    send_message(MSG_TYPE_INIT, payload, strlen((const char*)payload));
}

void process_received_data(void) {
    __disable_interrupt();
    uint16_t start = RX_start;
    uint16_t end = RX_end;
    bool receiving_flag = receiving_message;
    __enable_interrupt();

    if (!receiving_flag && start != end) {
        RX_start = read_RX_buffer(start, end);
    }
}
