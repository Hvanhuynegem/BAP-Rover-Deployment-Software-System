/*
 * LanderComm.h file
 *
 * This file will include all functionality regarding communication with the lander. The SLIP protocol is being used as communication protocol.
 * Within this communication protocol we designed another communication protocol that makes it easier to parse received messages such that the handler can handle data more effectively.
 *
 * Author: Henri Vanhuynegem
 * created: 23/05/2024
 * Last edited: 10/06/2024
 *
 */

#ifndef LANDER_COMMUNICATION_H
#define LANDER_COMMUNICATION_H

#include <lander_communication_lib/lander_communication_protocol.h>
#include <lander_communication_lib/uart_communication.h>
#include <system_health_lib/temp_sensors.h>
#include <msp430.h>
#include <cstdint>

// External global variables
extern bool ack_received;

/*
 * This method serializes the Message struct such that the data is entered into a buffer
 *
 * parameters:
 *  const Message* msg : message address
 *  uint8_t* buffer: buffer address
 *  uint8_t* length: length of serialized message
 *
 * Returns:
 *  bool : success status
 */
void convert_message_to_array(const Message* msg, uint8_t* buffer, uint8_t* length);

/*
 * This method deserializes the buffer into a Message struct
 *
 * parameters:
 *  const uint8_t* buffer: buffer address
 *  uint16_t length: length of buffer
 *  Message* msg: pointer to Message structure to be filled
 *
 * Returns:
 *  bool : success status
 */
bool convert_array_to_message(const uint8_t* buffer, uint16_t length, Message* msg);

/*
 * SLIP encoding.
 *
 * parameters:
 *  const uint8_t *buffer: address of buffer
 *  uint16_t length: length of buffer
 *  uint8_t *output_buffer: address of new array with encoded data
 *  uint16_t *received_length: output length of encoded array
 *
 * Returns:
 *  bool : success status
 */
bool slip_encode(const uint8_t *buffer, uint16_t length, uint8_t *output_buffer, uint16_t *received_length);

/*
 * SLIP decoding.
 *
 * parameters:
 *  const uint8_t *input_buffer: address of input buffer
 *  uint16_t input_length: length of input buffer
 *  uint8_t *output_buffer: address of new array with decoded data
 *  uint16_t *output_length: length of decoded array
 *
 * Returns:
 *  bool : success status
 */
bool slip_decode(const uint8_t *input_buffer, uint16_t input_length, uint8_t *output_buffer, uint16_t *output_length);

/*
 * Send a message structure using UART TX
 *
 * parameters:
 *  const Message* msg: message to be sent via UART TXD pin
 */
void send_message_struct(const Message* msg);

/*
 * Send a message using UART TX
 * parameters:
 *  uint8_t msg_type : message type
 *  const uint8_t *payload: pointer to array to be sent
 *  uint8_t length: length of array to be sent
 */
void send_message(uint8_t msg_type, const uint8_t *payload, uint8_t length);

/*
 * Sends a message and waits for an ACK response.
 *
 * Parameters:
 *  uint8_t msg_type : message type
 *  const uint8_t *payload : pointer to array to be sent
 *  uint8_t length : length of array to be sent
 *
 * Returns:
 *  void
 */
void send_message_and_wait_for_ACK(uint8_t msg_type, const uint8_t *payload, uint8_t length);

/*
 * process the RX_buffer
 */
void process_received_data(void);

#endif // LANDER_COMMUNICATION_H
