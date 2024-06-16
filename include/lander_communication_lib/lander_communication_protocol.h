/*
 * lander_communication_protocol.h file
 *
 * This file includes the internal communication protocol for communication with the lander. It is a self-made protocol that uses message type codes to handle incoming data more efficiently.
 *
 * Author: Henri Vanhuynegem
 * created: 23/05/2024
 * Last edited: 3/06/2024
 *
 */

#ifndef LANDER_COMMUNICATION_PROTOCOL_H
#define LANDER_COMMUNICATION_PROTOCOL_H


#include <cstdint>

// Message type codes
#define MSG_TYPE_INIT           0x01
#define MSG_TYPE_ACK            0x02
#define MSG_TYPE_NACK           0x03
#define MSG_TYPE_REQUEST        0x04
#define MSG_TYPE_DATA           0x05
#define MSG_TYPE_RESPONSE       0x06
#define MSG_TYPE_DEPLOY         0x07
#define MSG_TYPE_TRANSIT_MODE   0x08
#define MSG_TYPE_ERROR          0x09

#define MSG_START_BYTE  0x7E
#define MSG_END_BYTE    0x7F

#define MAX_PAYLOAD_SIZE 249  // UART buffer size - 7 (extra bytes)

// Message structure
typedef struct {
    uint8_t start_byte;
    uint8_t msg_type;
    uint8_t length;
    uint8_t payload[MAX_PAYLOAD_SIZE]; // pointer to payload array
    uint8_t checksum;
    uint8_t end_byte;
} Message;

/**
 * enumerate object that will be used for the different transit states
 */
typedef enum {
    GENERAL_STARTUP,
    LAUNCH_INTEGRATION,
    TRANSIT,
    PRE_DEPLOYMENT,
    DEPLOYMENT
} transit_states;

/* Tracks the current transit state */
extern volatile transit_states transit_state;

/*
 * Creates a message and returns the Message struct.
 *
 * Parameters:
 *  uint8_t msg_type : message type
 *  const uint8_t *payload: pointer to array to be sent
 *  uint8_t length: length of array to be sent
 *
 * Returns:
 *  Message: the constructed message struct
 */
Message create_message(uint8_t msg_type, const uint8_t *payload, uint8_t length);

/*
 * Reads a message struct and handles each type of response using a switch statement.
 *
 * Parameters:
 *  const Message *msg: message to be handled
 */
void handle_message(const Message *msg);

/*
 * Calculates the checksum of a message to check for errors during transmission.
 *
 * Parameters:
 *  uint8_t msg_type: message type
 *  uint8_t length: length of the message
 *  const uint8_t *payload: the message to be used for calculating a checksum
 *
 * Returns:
 *  uint8_t: the calculated checksum
 */
uint8_t calculate_checksum_helper(uint8_t msg_type, uint8_t length, const uint8_t *payload);

/*
 * Calculates the checksum of a message to check for errors during transmission.
 *
 * Parameters:
 *  const Message *msg : message structure
 *
 * Returns:
 *  uint8_t: the calculated checksum
 */
uint8_t calculate_checksum(const Message *msg);

#endif // LANDER_COMMUNICATION_PROTOCOL_H
