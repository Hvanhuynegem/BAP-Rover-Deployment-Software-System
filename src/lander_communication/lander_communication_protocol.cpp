/*
 * lander_communication_protocol.cpp file
 *
 * This file includes the internal communication protocol for communication with the lander. It is a self-made protocol that uses message type codes to handle incoming data more efficiently.
 *
 * Author: Henri Vanhuynegem
 * created: 23/05/2024
 * Last edited: 3/06/2024
 *
 */

#include <lander_communication_lib/lander_communication.h>
#include <lander_communication_lib/lander_communication_protocol.h>
#include <cstring>
#include <msp430.h>

/* transit state definition */
volatile transit_states transit_state;

// Helper function to calculate checksum
uint8_t calculate_checksum_helper(uint8_t msg_type, uint8_t length, const uint8_t *payload) {
    uint8_t checksum = msg_type ^ length;
    for (uint8_t i = 0; i < length; i++) {
        checksum ^= payload[i];
    }
    return checksum;
}

Message create_message(uint8_t msg_type, const uint8_t *payload, uint8_t length) {
    Message msg;
    msg.start_byte = MSG_START_BYTE;
    msg.msg_type = msg_type;
    msg.length = length;

    if(payload != nullptr)
    {
        memcpy(msg.payload, payload, length);
    }


    // Calculate checksum
    msg.checksum = calculate_checksum_helper(msg_type, length, payload);
    msg.end_byte = MSG_END_BYTE;
    return msg;
}

uint8_t calculate_checksum(const Message *msg){
    return calculate_checksum_helper(msg->msg_type, msg->length, msg->payload);
}

void handle_message(const Message *msg) {
    if (msg->start_byte != MSG_START_BYTE || msg->end_byte != MSG_END_BYTE) {
        // Invalid message
        uint8_t payload[] = "INVALID_MESSAGE";
        send_message(MSG_TYPE_ERROR, payload, strlen((const char*)payload));
        return;
    }

    if (msg->checksum != calculate_checksum(msg)) {
        // Invalid checksum
        uint8_t payload[] = "INVALID_CHECKSUM";
        send_message(MSG_TYPE_ERROR, payload, strlen((const char*)payload));
        return;
    }

    switch (msg->msg_type) {
        case MSG_TYPE_INIT:
            // Handle initialization sequence
            uint8_t ack_payload[] = "ACK";
            send_message(MSG_TYPE_ACK, ack_payload, strlen((const char*)ack_payload));
            break;
        case MSG_TYPE_ACK:
            // Handle acknowledgment
            ack_received = true;
            break;
        case MSG_TYPE_REQUEST:
            // Handle request
            break;
        case MSG_TYPE_DATA:
            // Handle data
            break;
        case MSG_TYPE_RESPONSE:
            // Handle response
            break;
        case MSG_TYPE_DEPLOY:
            transit_state = DEPLOYMENT;
            break;
        case MSG_TYPE_TRANSIT_MODE:
            if (msg->payload[0] == 'G' && msg->payload[1] == 'S') { // general startup (GS)
                transit_state = GENERAL_STARTUP;
            } else if (msg->payload[0] == 'L' && msg->payload[1] == 'I') { // Launch Integration (LI)
                transit_state = LAUNCH_INTEGRATION;
            } else if (msg->payload[0] == 'T') { // Transit (T)
                transit_state = TRANSIT;
            } else if (msg->payload[0] == 'P' && msg->payload[1] == 'D') { // Pre-Deployment (PD)
                transit_state = PRE_DEPLOYMENT;
            } else if (msg->payload[0] == 'D') { // Deployment (D)
                transit_state = DEPLOYMENT;
            } else {
                // Handle unknown state or do nothing
            }

            break;
        case MSG_TYPE_ERROR:
            // Handle mode
            break;
        default:
            // Unknown message type
            break;
    }
}
