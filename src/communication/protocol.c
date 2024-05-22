#include "protocol.h"
#include "landerComm.h"
#include <string.h>
#include <msp430.h>

void send_message(uint8_t msg_type, const uint8_t *payload, uint8_t length) {
    Message msg;
    msg.start_byte = START_BYTE;
    msg.msg_type = msg_type;
    msg.length = length;
    memcpy(msg.payload, payload, length);
    msg.checksum = calculate_checksum(&msg);
    msg.end_byte = END_BYTE;

    // Encode and send the message
    slip_encode((uint8_t *)&msg, sizeof(msg.start_byte) + sizeof(msg.msg_type) + sizeof(msg.length) + length + sizeof(msg.checksum) + sizeof(msg.end_byte));
}

void handle_message(const Message *msg) {
    if (msg->start_byte != START_BYTE || msg->end_byte != END_BYTE) {
        // Invalid message
        P1OUT |= BIT0;  // Turn on LED to indicate invalid message
        return;
    }

    if (msg->checksum != calculate_checksum(msg)) {
        // Invalid checksum
        P1OUT |= BIT0;  // Turn on LED to indicate invalid checksum
        return;
    }

    switch (msg->msg_type) {
        case MSG_TYPE_INIT:
            // Handle initialization sequence
//            P1OUT ^= BIT0;  // Toggle LED to indicate handling MSG_TYPE_INIT
            send_message(MSG_TYPE_ACK, NULL, 0);
            break;
        case MSG_TYPE_ACK:
            // Handle acknowledgment
//            P1OUT ^= BIT0;  // Toggle LED to indicate handling MSG_TYPE_ACK
            break;
        case MSG_TYPE_REQUEST:
            // Handle request
//            P1OUT ^= BIT0;  // Toggle LED to indicate handling MSG_TYPE_REQUEST
            send_message(MSG_TYPE_RESPONSE, (const uint8_t *)"Response data", 13);
            break;
        case MSG_TYPE_DATA:
            // Handle data
//            P1OUT ^= BIT0;  // Toggle LED to indicate handling MSG_TYPE_DATA
            break;
        case MSG_TYPE_RESPONSE:
            // Handle response
//            P1OUT ^= BIT0;  // Toggle LED to indicate handling MSG_TYPE_RESPONSE
            break;
        default:
            // Unknown message type
//            P1OUT ^= BIT0;  // Toggle LED to indicate handling unknown message type
            break;
    }
}

uint8_t calculate_checksum(const Message *msg) {
    uint8_t checksum = 0;
    checksum ^= msg->msg_type;
    checksum ^= msg->length;
    for (uint8_t i = 0; i < msg->length; i++) {
        checksum ^= msg->payload[i];
    }
    return checksum;
}
