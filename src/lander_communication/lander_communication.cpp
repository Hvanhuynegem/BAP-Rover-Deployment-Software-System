/*
 * lander_communication.cpp file
 *
 * This file will include all functionality regarding communication with the lander. The SLIP protocol is being used as communication protocol.
 * Within this communication protocol we designed another communication protocol that makes it easier to parse received messages such that the handler can handle data more effectively.
 *
 * Author: Henri Vanhuynegem
 * created: 23/05/2024
 * Last edited: 10/06/2024
 *
 */

#include <lander_communication_lib/lander_communication.h>
#include <cstring>

// Global variables
bool ack_received = false;


void convert_message_to_array(const Message* msg, uint8_t* buffer, uint8_t* length) {

    uint16_t index = 0;

    // Copy start_byte
    buffer[index++] = msg->start_byte;

    // Copy msg_type
    buffer[index++] = msg->msg_type;

    // Copy length and ensure msg length is smaller than max payload size
    if(msg->length > MAX_PAYLOAD_SIZE){
        buffer[index++] = MAX_PAYLOAD_SIZE;
        // Copy payload
        memcpy(&buffer[index], msg->payload, MAX_PAYLOAD_SIZE);
        index += MAX_PAYLOAD_SIZE;
    } else {
        buffer[index++] = msg->length;
        // Copy payload
        memcpy(&buffer[index], msg->payload, msg->length);
        index += msg->length;
    }

    // Copy checksum
    buffer[index++] = msg->checksum;

    // Copy end_byte
    buffer[index++] = msg->end_byte;

    // Set the total length of the serialized message
    *length = index;
}


bool convert_array_to_message(const uint8_t* buffer, uint16_t length, Message* msg) {
    if (length < 5) {
        return false; // Handle invalid length
    }

    uint16_t index = 0;

    // Extract start_byte
    msg->start_byte = buffer[index++];

    // Extract msg_type
    msg->msg_type = buffer[index++];

    //temp length
    uint8_t temp_length;
    temp_length = buffer[index++];

    // Check if the length is valid
    if (temp_length > MAX_PAYLOAD_SIZE) {
        // Extract length
        temp_length = buffer[index++];
        msg->length = MAX_PAYLOAD_SIZE;
        // Extract payload
        memcpy(msg->payload, &buffer[index], MAX_PAYLOAD_SIZE);
        index += temp_length;
    } else {
        // Extract length
        msg->length = temp_length;
        // Extract payload
        memcpy(msg->payload, &buffer[index], msg->length);
        index += msg->length;
    }

    // Extract checksum
    msg->checksum = buffer[index++];

    // Extract end_byte
    msg->end_byte = buffer[index++];

    return true;
}


bool slip_encode(const uint8_t *buffer, uint16_t length, uint8_t *output_buffer, uint16_t *received_length) {
    // definition of slip encoding characters
    const uint8_t END = 0xC0;
    const uint8_t ESC = 0xDB;
    const uint8_t ESC_END = 0xDC;
    const uint8_t ESC_ESC = 0xDD;

    uint16_t index = 0;
    output_buffer[index++] = END;

    for (uint16_t i = 0; i < length; i++) {
        switch (buffer[i]) {
            case END:
                if (index + 2 >= UART_BUFFER_SIZE) {
                    // Prevent overflow if there is not enough space for ESC and ESC_END
                    return false;
                }
                output_buffer[index++] = ESC;
                output_buffer[index++] = ESC_END;
                break;
            case ESC:
                if (index + 2 >= UART_BUFFER_SIZE) {
                    // Prevent overflow if there is not enough space for ESC and ESC_ESC
                    return false;
                }
                output_buffer[index++] = ESC;
                output_buffer[index++] = ESC_ESC;
                break;
            default:
                if (index >= UART_BUFFER_SIZE - 1) {
                    // Prevent overflow if there is not enough space for the byte and the final END
                    return false;
                }
                output_buffer[index++] = buffer[i];
        }
    }

    output_buffer[index++] = END;

    *received_length = index;
    return true;
}


bool slip_decode(const uint8_t *input_buffer, uint16_t input_length, uint8_t *output_buffer, uint16_t *output_length) {
    // Check input length and start/end bytes
    if (input_length < 2 || input_buffer[0] != 0xC0 || input_buffer[input_length - 1] != 0xC0) {
        return false;
    }
    // check if data is within acceptable buffer size
    if (input_length > UART_BUFFER_SIZE){
        return false;
    }
    // Definition of slip encoding characters
    const uint8_t END = 0xC0;
    const uint8_t ESC = 0xDB;
    const uint8_t ESC_END = 0xDC;
    const uint8_t ESC_ESC = 0xDD;

    bool is_escaped = false;
    *output_length = 0;

    // Run through buffer to replace potential END and ESC
    for (uint16_t i = 1; i < input_length - 1; i++) {
        uint8_t c = input_buffer[i];

        if (is_escaped) {
            if (c == ESC_END) {
                c = END;
            } else if (c == ESC_ESC) {
                c = ESC;
            } else {
                return false;
            }
            is_escaped = false;
        } else if (c == ESC) {
            is_escaped = true;
            continue;
        }

        output_buffer[(*output_length)++] = c;
    }
    return true;
}



void send_message_struct(const Message* msg) {
    // Static buffer to hold serialized and encoded data
    static uint8_t buffer[UART_BUFFER_SIZE];
    static uint8_t temp_buffer[UART_BUFFER_SIZE];
    uint8_t serialized_length;
    uint16_t encoded_length;
    bool result;

    // Serialize the message into the first part of the buffer
    convert_message_to_array(msg, temp_buffer, &serialized_length);

    // Encode the message using SLIP into the second part of the buffer
    result = slip_encode(temp_buffer, serialized_length, buffer, &encoded_length);
    if (!result) {
        // Handle encoding failure
        return;
    }

    // Send the encoded message
    uart_write(buffer, encoded_length);
}


void send_message(uint8_t msg_type, const uint8_t *payload, uint8_t length){
    // Create a message structure
    Message msg = create_message(msg_type, payload, length);

    // Send the message
    send_message_struct(&msg);
}

void send_message_and_wait_for_ACK(uint8_t msg_type, const uint8_t *payload, uint8_t length){
    // Initialize connection with the lander
    bool acknowledgement = false;
    while(!acknowledgement){
        // send an initialisation message to the lander
        send_message(msg_type, payload, length);
        // start a timeout using timer TA2
        startTimeoutTimer_TA2();
        // wait until timeout is done
        while(!timeoutOccurred){}

        // check whether the response is an ACK
        process_received_data();
        if(ack_received){
            acknowledgement = true;
            ack_received = false;
        }
    }
}

void process_received_data(void) {
    __disable_interrupt();
    uint16_t start = RX_start;
    uint16_t end = RX_end;
    UART_states state = UART_state;
    __enable_interrupt();

    if (UART_state == RECEIVED && start != end) {
        UART_state = IDLE;
        RX_start = read_RX_buffer(start, end);
    } else if (buffer_full_state){
        // Create a ERROR message
        uint8_t payload[] = "MESSAGE_TOO_LARGE";
        send_message(MSG_TYPE_ERROR, payload, strlen((const char*)payload));
        buffer_full_state = false;
    } else if(error_state){
        // Create a ERROR message
        uint8_t payload[] = "";
        send_message(MSG_TYPE_ERROR, payload, strlen((const char*)payload));
        error_state = false;
    } else if (timeout_state) {
        // Create a NACK message
        uint8_t payload[] = "";
        send_message(MSG_TYPE_NACK, payload, strlen((const char*)payload));
        timeout_state = false;
    } else {}
}

