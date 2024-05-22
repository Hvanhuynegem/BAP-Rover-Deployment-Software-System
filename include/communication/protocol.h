#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

// Message type codes
#define MSG_TYPE_INIT        0x01
#define MSG_TYPE_ACK         0x02
#define MSG_TYPE_REQUEST     0x03
#define MSG_TYPE_DATA        0x04
#define MSG_TYPE_RESPONSE    0x05

#define START_BYTE  0x7E
#define END_BYTE    0x7F

// Message structure
typedef struct {
    uint8_t start_byte;
    uint8_t msg_type;
    uint8_t length;
    uint8_t payload[256]; // Adjust size as needed
    uint8_t checksum;
    uint8_t end_byte;
} Message;

// Function prototypes
void send_message(uint8_t msg_type, const uint8_t *payload, uint8_t length);
void handle_message(const Message *msg);
uint8_t calculate_checksum(const Message *msg);

#endif // PROTOCOL_H
