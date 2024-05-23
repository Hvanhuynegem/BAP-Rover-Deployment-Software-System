/*
 * LanderCommProtocol.h file
 *
 * This file includes the internal communication protocol for communication with the lander. It is a self-made protocol that uses message type codes to handle incoming data more efficiently.
 *
 * Author: Henri Vanhuynegem
 * Date: 23/05/2024
 *
 */

#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

// Message type codes
#define MSG_TYPE_INIT        0x01
#define MSG_TYPE_ACK         0x02
#define MSG_TYPE_REQUEST     0x03
#define MSG_TYPE_DATA        0x04
#define MSG_TYPE_RESPONSE    0x05
#define MSG_TYPE_DEPLOYMENT  0x06
#define MSG_TYPE_MODE        0x07

#define START_BYTE  0x7E
#define END_BYTE    0x7F

// Define payload size
#define PAYLOAD_SIZE 32

// Message structure
typedef struct {
    uint8_t start_byte;
    uint8_t msg_type;
    uint8_t length;
    uint8_t payload[PAYLOAD_SIZE]; // Adjust size as needed
    uint8_t checksum;
    uint8_t end_byte;
} Message;

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
 *  const Message *msg: the message to be used for calculating a checksum
 *
 * Returns:
 *  uint8_t: the calculated checksum
 */
uint8_t calculate_checksum(const Message *msg);

/*
 * Calculates the checksum of a message to check for errors during transmission.
 *
 * Parameters:
 *  const uint8_t *buffer: the message to be used for calculating a checksum
 *  uint8_t length: length of the message
 *
 * Returns:
 *  uint8_t: the calculated checksum
 */
uint8_t calculate_checksum(const uint8_t *buffer, uint8_t length);

#endif // PROTOCOL_H
