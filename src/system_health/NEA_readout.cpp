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

// Function to activate a specific NEA
void activate_NEA_n(int nea) {
    switch (nea) {
        case 1:
            P1OUT |= BIT0;                  // Activate NEA 1 (set P1.0 to high)
            break;
        case 2:
            P1OUT |= BIT1;                  // Activate NEA 2 (set P1.1 to high)
            break;
        case 3:
            P1OUT |= BIT2;                  // Activate NEA 3 (set P1.2 to high)
            break;
        case 4:
            P3OUT |= BIT0;                  // Activate NEA 4 (set P3.0 to high)
            break;
        default:
            // Handle invalid NEA number
            break;
    }
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
}

// Function to deactivate a specific NEA
void deactivate_NEA_n(int nea) {
    switch (nea) {
        case 1:
            P1OUT &= ~BIT0;                 // Deactivate NEA 1 (set P1.0 to low)
            break;
        case 2:
            P1OUT &= ~BIT1;                 // Deactivate NEA 2 (set P1.1 to low)
            break;
        case 3:
            P1OUT &= ~BIT2;                 // Deactivate NEA 3 (set P1.2 to low)
            break;
        case 4:
            P3OUT &= ~BIT0;                 // Deactivate NEA 4 (set P3.0 to low)
            break;
        default:
            // Handle invalid NEA number
            break;
    }
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
}

void activate_NEAs(void){
    int k = 0;
    for(int i=0; i<4 ; i++){
        while(k < 3){
            if(supercap_functionality[0]){
                switch_on_charge_cap_flag(0);
            }
            if(supercap_functionality[1]){
                switch_on_charge_cap_flag(1);
            }
            if(supercap_functionality[2]){
                switch_on_charge_cap_flag(2);
            }
            //timer 3 min = 180 seconds = 180x4 = 0.25 seconds x 720
            startTimeoutTimer_TA3();
            while(timeoutCounterTA3 < 720){
                process_received_data(); // read the RX buffer while waiting for timer
            } // 720 times 0.25 seconds is 3 minutes
            stopTimeoutTimer_TA3();

            // activate NEA x
            activate_NEA_n(i);

            // timer of 0.25 seconds
            startTimeoutTimer_TA3();
            while(timeoutCounterTA3 < 1){
                process_received_data(); // read the RX buffer while waiting for timer
            } // 1 times 0.25 seconds is 0.25 seconds
            stopTimeoutTimer_TA3();

            // check if NEA x detonated
            bool status_NEA_n = true;
            if(i == 0){
                status_NEA_n = read_NEAready_status(&P3IN, BIT1);
            } else if (i == 1){
                status_NEA_n = read_NEAready_status(&P3IN, BIT2);
            } else if (i == 2){
                status_NEA_n = read_NEAready_status(&P3IN, BIT3);
            } else if (i == 3){
                status_NEA_n = read_NEAready_status(&P4IN, BIT7);
            } else {}

            // send a message to lander
            send_NEA_message(i, status_NEA_n);

            if(status_NEA_n){
                k++;
            } else {
                k = 3;
            }
        }
        deactivate_NEA_n(i);
    }

}

void send_NEA_message(int nea, bool status){
    if (nea == 0) {
        if(status){
            // Send message that NEA 1 is not activated yet
            send_message(MSG_TYPE_DATA, PAYLOAD_NEA1_READY, sizeof(PAYLOAD_NEA1_READY) - 1);
        } else {
            // Send message that NEA 1 is already activated
            send_message(MSG_TYPE_DATA, PAYLOAD_NEA1_NOT_READY, sizeof(PAYLOAD_NEA1_NOT_READY) - 1);
        }
    }
    if (nea == 1) {
        if(status){
            // Send message that NEA 2 is not activated yet
            send_message(MSG_TYPE_DATA, PAYLOAD_NEA2_READY, sizeof(PAYLOAD_NEA2_READY) - 1);
        } else {
            // Send message that NEA 2 is already activated
            send_message(MSG_TYPE_DATA, PAYLOAD_NEA2_NOT_READY, sizeof(PAYLOAD_NEA2_NOT_READY) - 1);
        }
    }
    if (nea == 2) {
        if(status){
            // Send message that NEA 3 is not activated yet
            send_message(MSG_TYPE_DATA, PAYLOAD_NEA3_READY, sizeof(PAYLOAD_NEA3_READY) - 1);
        } else {
            // Send message that NEA 3 is already activated
            send_message(MSG_TYPE_DATA, PAYLOAD_NEA3_NOT_READY, sizeof(PAYLOAD_NEA3_NOT_READY) - 1);
        }
    }
    if (nea == 3) {
        if(status){
            // Send message that NEA 4 is not activated yet
            send_message(MSG_TYPE_DATA, PAYLOAD_NEA4_READY, sizeof(PAYLOAD_NEA4_READY) - 1);
        } else {
            // Send message that NEA 4 is already activated
            send_message(MSG_TYPE_DATA, PAYLOAD_NEA4_NOT_READY, sizeof(PAYLOAD_NEA4_NOT_READY) - 1);
        }
    }
}


// Function to read the status of the NEA ready bit
bool read_NEAready_status(volatile uint8_t *port_in, uint8_t pin) {
    return (*port_in & pin) != 0;  // Return the status of the specified pin
}
