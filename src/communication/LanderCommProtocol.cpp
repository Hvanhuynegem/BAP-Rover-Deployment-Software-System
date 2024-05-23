/*
 * LanderCommProtocol.cpp file
 *
 * This file includes the internal communication protocol for communication with the lander. It is a self-made protocol that uses message type codes to handle incoming data more efficiently.
 *
 * Author: Henri Vanhuynegem
 * Date: 23/05/2024
 *
 */

#include "communication/landerComm.h"
#include "communication/landerCommProtocol.h"
#include <string.h>
#include <msp430.h>

// Helper function to calculate checksum
uint8_t calculate_checksum(const uint8_t *buffer, uint8_t length) {
    uint8_t checksum = 0;
    for (uint8_t i = 0; i < length; i++) {
        checksum ^= buffer[i];
    }
    return checksum;
}

Message create_message(uint8_t msg_type, const uint8_t *payload, uint8_t length) {
    Message msg;
    msg.start_byte = START_BYTE;
    msg.msg_type = msg_type;
    msg.length = length;

    // Initialize payload with zeros
    memset(msg.payload, 0, PAYLOAD_SIZE);

    // Copy payload data
    if (payload != NULL && length <= PAYLOAD_SIZE) {
        memcpy(msg.payload, payload, length);
    }

    // Calculate checksum
    msg.checksum = calculate_checksum((const uint8_t *)&msg, sizeof(msg.start_byte) + sizeof(msg.msg_type) + sizeof(msg.length) + PAYLOAD_SIZE);
    msg.end_byte = END_BYTE;

    return msg;
}


uint8_t calculate_checksum(const Message *msg) {
    return calculate_checksum((const uint8_t *)msg, sizeof(msg->start_byte) + sizeof(msg->msg_type) + sizeof(msg->length) + msg->length);
}

void handle_message(const Message *msg) {
    if (msg->start_byte != START_BYTE || msg->end_byte != END_BYTE) {
        // Invalid message
        P1OUT ^= BIT0; // Toggle LED as an error indicator
        return;
    }

    if (msg->checksum != calculate_checksum(msg)) {
        // Invalid checksum
        P1OUT ^= BIT0; // Toggle LED as an error indicator
        return;
    }

    switch (msg->msg_type) {
        case MSG_TYPE_INIT:
            // Handle initialization sequence
            {
                uint8_t ack_payload[] = "ACK";
                Message ack_msg = create_message(MSG_TYPE_ACK, ack_payload, sizeof(ack_payload));
                send_message(&ack_msg);
            }
            break;
        case MSG_TYPE_ACK:
            // Handle acknowledgment
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
        case MSG_TYPE_DEPLOYMENT:
            // Handle deployment
            break;
        case MSG_TYPE_MODE:
            // Handle mode
            break;
        default:
            // Unknown message type
            break;
    }
}
