/*
 * LanderComm.h file
 *
 * This file will include all functionality regarding communication with the lander. The SLIP protocol is being used as communication protocol.
 * Within this communication protocol we designed another communication protocol that makes it easier to parse received messages such that the handler can handle data more effectively.
 *
 * Author: Henri Vanhuynegem
 * Date: 23/05/2024
 *
 */
#ifndef LANDER_COMM_H
#define LANDER_COMM_H

#include <msp430.h>
#include <stdint.h>
#include "landerCommProtocol.h"

// UART buffers and state variables
#define UART_BUFFER_SIZE 256

extern uint8_t TX_buffer[UART_BUFFER_SIZE];
extern volatile uint16_t TX_start;
extern volatile uint16_t TX_end;
extern uint8_t RX_buffer[UART_BUFFER_SIZE];
extern volatile uint16_t RX_start;
extern volatile uint16_t RX_end;

/*
 * Initializes the UART ports for pins 2.5 and 2.6 at a baud rate of 9600 baud/s.
 */
void initialize_UART_A1(void);

/*
 * Initializes the clock (ACLK) at frequency 32.768Hz.
 */
void initialize_clock(void);

/*
 * SLIP encoding.
 *
 * parameters:
 *  const uint8_t *buffer: address of buffer
 *  uint16_t length: length of buffer
 *  uint8_t *output_buffer: address of new array with data
 *  uint16_t *received_length: output length of encoded array
 */
void slip_encode(const uint8_t *buffer, uint16_t length, uint8_t *output_buffer, uint16_t *received_length);

/*
 * SLIP decoding.
 *
 * parameters:
 *  const uint8_t *buffer: address of buffer
 *  uint16_t *received_length: length of output
 *  uint8_t *output_buffer: address of new array with data
 */
void slip_decode(const uint8_t *input_buffer, uint16_t input_length, uint8_t *output_buffer, uint16_t *output_length);

/*
 * Send a message using UART
 *
 * parameters:
 *  const Message* msg: message to be sent via UART TXD pin
 */
void send_message(const Message* msg);

/*
 * This method is able to send an array of data through the UART output pin by using the add to TX buffer method.
 *
 * parameters:
 *  const uint8_t* data: This is the address of array to be sent.
 *  uint16_t length: This is the length of the array to be sent.
 */
void uart_send_data(const uint8_t* data, uint16_t length);

/*
 * This method puts a byte into the transmission buffer of the output pin 2.5
 *
 * parameters:
 *  uint8_t byte: The byte to be sent.
 */
void add_to_TX_buffer(uint8_t byte);

/*
 * This method enables the transmission interrupts again when the buffer is filled and tx_start is not equal to tx_end
 */
void process_TX_buffer(void);

#endif // LANDER_COMM_H
