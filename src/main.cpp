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
#include "lander_communication_lib/payload_messages.h"
#include "system_health_lib/main_system_init.h"
#include "transit_modes_lib/general_startup.h"
#include "transit_modes_lib/launch_mode.h"
#include "transit_modes_lib/transit_mode.h"
#include "transit_modes_lib/pre_deployment_mode.h"
#include "transit_modes_lib/deployment_mode.h"


int main(void) {
    // Stop watchdog timer
    WDTCTL = WDTPW | WDTHOLD;

    // initialise everything
    boot_up_initialisation();

    // Pins start in high-impedance mode on wake-up, this enables them again
    // and activates our port configurations
    PM5CTL0 &= ~LOCKLPM5;

    // enable global interrupts
    __bis_SR_register(GIE);

    // Main loop (not used in this example)
    while (1) {
        switch (transit_state){
        case GENERAL_STARTUP:
            send_message(MSG_TYPE_RESPONSE, PAYLOAD_GENERAL_STARTUP, sizeof(PAYLOAD_GENERAL_STARTUP) - 1);
            general_startup();
            break;
        case LAUNCH_INTEGRATION:
            send_message(MSG_TYPE_RESPONSE, PAYLOAD_LAUNCH_INTEGRATION, sizeof(PAYLOAD_LAUNCH_INTEGRATION) - 1);
            launch_mode();
            break;
        case TRANSIT:
            send_message(MSG_TYPE_RESPONSE, PAYLOAD_TRANSIT, sizeof(PAYLOAD_TRANSIT) - 1);
            transit_mode();
            break;
        case PRE_DEPLOYMENT:
            send_message(MSG_TYPE_RESPONSE, PAYLOAD_PRE_DEPLOYMENT, sizeof(PAYLOAD_PRE_DEPLOYMENT) - 1);
            pre_deployment_mode();
            break;
        case DEPLOYMENT:
            send_message(MSG_TYPE_RESPONSE, PAYLOAD_DEPLOYMENT, sizeof(PAYLOAD_DEPLOYMENT) - 1);
            deployment_mode();
            break;
        default:
            // process RX buffer
            process_received_data();
            general_startup();
            break;
        }
    }
}


