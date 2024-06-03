/*
 * LanderCommProtocol.cpp file
 *
 * This file includes the internal communication protocol for communication with the lander. It is a self-made protocol that uses message type codes to handle incoming data more efficiently.
 *
 * Author: Henri Vanhuynegem
 * created: 23/05/2024
 * Last edited: 3/06/2024
 *
 */

#include "communication/landerComm.h"
#include "communication/landerCommProtocol.h"
#include <string.h>
#include <msp430.h>

// Helper function to calculate checksum
uint8_t calculate_checksum_helper(const uint8_t *buffer, uint8_t length) {
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

    // Initialize payload vector and copy data
    msg.payload.resize(length);
    if (payload != NULL && length > 0) {
        std::copy(payload, payload + length, msg.payload.begin());
    }

    // Calculate checksum
    std::vector<uint8_t> buffer;
    buffer.push_back(msg.start_byte);
    buffer.push_back(msg.msg_type);
    buffer.push_back(msg.length);
    buffer.insert(buffer.end(), msg.payload.begin(), msg.payload.end());
    msg.checksum = calculate_checksum_helper(buffer.data(), buffer.size());
    msg.end_byte = END_BYTE;

    return msg;
}


uint8_t calculate_checksum(const Message *msg) {
    std::vector<uint8_t> buffer;
    buffer.push_back(msg->start_byte);
    buffer.push_back(msg->msg_type);
    buffer.push_back(msg->length);
    buffer.insert(buffer.end(), msg->payload.begin(), msg->payload.end());
    return calculate_checksum_helper(buffer.data(), buffer.size());
}

void handle_message(const Message *msg) {
    if (msg->start_byte != START_BYTE || msg->end_byte != END_BYTE) {
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
