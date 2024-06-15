/*
 * uart_communication.h file
 *
 * This is the UART communication library that contains the following functionality:
 *
 *  - starting, stopping, and resetting a timer
 *  - Initialising and configure the UART pins and states
 *  - Enter data into UART transmission buffer
 *  - UART interrupt handler
 *  - ISR for A0 and A1 UART modules
 *  - ISR for timer A0 module
 *
 *  The library works using interrupts for transmission and receiving of characters via the UART ports.
 *  It initialises the system to be used on the USCI_A1 module, since the PCB design of the RDS system
 *  routes output pins 2.5 and 2.6 to the RS-422 transceiver.
 *
 *
 *
 * Author: Henri Vanhuynegem
 * created: 5/06/2024
 * Last edited: 5/06/2024
 *
 */

#ifndef UART_COMM_H
#define UART_COMM_H

// included packages and libraries
//#include <msp430.h>
#include <cstdint>
#include "lander_communication_protocol.h"
#include "lander_communication.h"

// UART BUFFER SIZE definition
#define UART_BUFFER_SIZE 256

// global UART buffers and state variables
extern uint8_t TX_buffer[UART_BUFFER_SIZE];
extern volatile uint16_t TX_start;
extern volatile uint16_t TX_end;
extern uint8_t RX_buffer[UART_BUFFER_SIZE];
extern volatile uint16_t RX_start;
extern volatile uint16_t RX_end;

/**
 * enumerate object that will be used for the finite state machine that handles incoming data
 */
typedef enum {
    IDLE,
    RECEIVING,
    RECEIVED,
    BUFFER_FULL,
    ERROR,
    TIMEOUT
} UART_states;

/* Tracks the current UART state */
extern volatile UART_states UART_state;


/**
 * Starts the timer that runs on TA0 module
 *
 */
inline static void start_timeout(void);

/**
 * resets the timer to 0 that runs on TA0 module
 *
 */
inline static void reset_timeout(void);

/**
 * Stops/Disables the timer that runs on TA0 module
 *
 */
inline static void stop_timeout(void);

/**
 * Configures the A1 module to initialise pins 2.6 (RX) and 2.5 (TX) as UART at a baud rate of 115200 baud/s.
 */
void uart_init(void);

/**
 * Configures the UART state and indexes of the buffers
 */
void uart_configure(void);

/*
 * This method is able to send an array of data through the UART output pin by adding the data to the transmission buffer.
 *
 * parameters:
 *  const uint8_t* data: This is the address of array to be sent.
 *  uint16_t length: This is the length of the array to be sent.
 */
void uart_write(uint8_t *data, uint16_t length);

/**
 * Generic handler for UART interrupts
 *
 * parameters:
 *  uint8_t character - the character to handle
 */
inline static void uart_interrupt_handler(uint8_t character);

/*
 * This method enables the transmission interrupts again when the buffer is filled and tx_start is not equal to tx_end
 */
inline void process_TX_buffer(void);

/*
 * This method handles the data in the RX buffer
 */
uint16_t read_RX_buffer(uint16_t start, uint16_t end);

/*
 * setup the smclk
 */
void setup_SMCLK(void);

#endif // UART_COMM_H
