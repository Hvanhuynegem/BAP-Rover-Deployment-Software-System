#ifndef LANDER_COMM_H
#define LANDER_COMM_H

#include <msp430.h>
#include <stdint.h>

/*
 * This function initializes the UART ports using a baud rate.
 *
 */
void initialize_UART(void);

/*
 * This function encodes an array and sends it through the two UART pins.
 *
 * parameters:
 *   uint8_t *buffer: message to be sent
 *   uint16_t length: length of packet to be sent
 */
void slip_encode(const uint8_t *buffer, uint16_t length);

/*
 * This function decodes an array and receives it through the two UART pins.
 *
 * parameters:
 *   uint8_t *buffer: message to be sent
 *   uint16_t *received_length: received length of received packet
 */
void slip_decode(uint8_t *buffer, uint16_t *received_length);

/*
 * This function initialises the ACLK at a frequency of 32.768 Hz
 *
 */
void initialize_clock(void);

/*
 * This function initialises a timer for a certain amount of clk cycles
 *
 * parameters:
 *  int cycles: the amount of cycles that need to be run.
 */
void start_timer(unsigned int cycles);

#endif // LANDER_COMM_H
