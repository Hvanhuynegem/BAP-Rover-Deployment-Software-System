/**
 * @file msp_serial.h
 * @author Dāvis Kažemaks
 * @version 1.1
 * @date 2023-05-24
 * @brief
 * This is the serial library that implements UART communication on MSP430FR5969.
 * For now, it is heavily integrated with the Bus Manager library and uses BLE-derived
 * messaging protocol.
 *
 * The Bus manager uses:
 *  - serial_close
 *  - serial_configure
 *  - serial_write
 *  - serial_read
 * to interface with the msp_serial library.
 *
 * The library works on interrupts and the state of the program is tracked by serial_state.
 * Whenever there is a byte on the bus, it triggers the interrupt, which will handle the incoming
 * byte. When the first byte on the bus is received, the program enters RECEIVING state, until
 * either: the full packet is received, the timeout is triggered, or the buffer exceeds maximum size.
 */

#ifndef UART_H_ISR
#define UART_H_ISR

#include "subsystems.h"
#include <msp430.h>
#include <packet.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @brief Indicates which module is being used for UART
 *   0 - A0 module that uses pins 2.1 (RX) and 2.0 (TX) (for MSP430 dev boards, this is also the USB back channel)
 *   1 - A1 module that uses pins 2.5 (tx) and 2.6 (rx)
 */
#define UART_MODULE 1

/**
 * @brief The amount of ticks that need to be reached for the timeout to trigger
 *   The tick rate is tied to SMCLK, which in current implementation is configured to ~16Mhz.
 *   The clock is further divided by 8 and only triggers a tick on the rising edge.
 *   The time for the timer to expire is calculated by  TIMEOUT_TICKS / (CLOCK_FREQUENCY / DIVIDER)
 *   Current timeout = 10000 / (16M / 8) = 10000 / 2M = 0.005s
 *
 */
#define DEFAULT_TIMEOUT_TICKS 10000

// Used to covert miliseconds into ticks (assuming clock is 16Mhz)
// Calculation: 2000 / (16M / 8) = 2000 / 2M = 0.001s
#define MS_TO_TICKS 2000

// Identifies the current subsystem. Used in by the bus manager to check DST field for correct destination
#define SUBSYSTEM_ID (subsystems::Subsystem_t::COMMS)

/**
 * @brief
    Serial states that indicate what is happening.
    If the state is PACKET_RECEIVED, TIMEOUT or BUFFER_FULL, no bytes will be added to the buffer
    until serial_read is called.
    IDLE - serial is inactive and the buffer is empty (message_pointer == 0)
    RECEIVING - serial is currently receiving a message
    PACKET_RECEIVED -a full packet has been received
    TIMEOUT - only part of the packet was received, so a timeout triggered before receiving the full packet
    BUFFER_FULL - the packet size went over the maximum size of the buffer (should not be possible)
 *
 */
typedef enum
{
    IDLE = 0,

    RECEIVING,
    PACKET_RECEIVED,
    TIMEOUT,
    BUFFER_FULL,
    INVALID_DESTINATION

} Serial_states_t;

/* Tracks current serial state*/
extern volatile Serial_states_t serial_state;

// Buses used by the Bus manager
// @todo should refactor bus manager to not use these with MSP
#define BUS0 "/dev/ttyUSB0"
#define BUS1 "/dev/ttyUSB1"
#define BUS2 "/dev/ttyUSB2"
#define BUS3 "/dev/ttyUSB3"
#define BUS4 "/dev/ttyUSB4"

/* Serial baudrate
    @todo currently unused, and most likely will not be used
    only here for the function signature
*/
typedef enum
{
    BAUD9600,
    BAUD115200
} Baudrate_t;

/* Serial function type (blocking or non blocking)
    @todo currently unused, and most likely will not be used
    only here for the function signature
*/
typedef enum
{
    SERIAL_BLOCKING,
    SERIAL_NONBLOCKING
} Serial_mode_t;

/**
 * @brief Closes the serial connection
 * @todo currently does nothing, here for the bus manager signature
 * @param serial_fd - unused
 * @return int32_t - always returns 0
 */
int32_t serial_close(int serial_fd);

/**
 * @brief Configures the serial library to use one of the modules A0 or A1 (this is changed by the #define)
 *
 * @param serial_port - unused
 * @param baudrate - unused
 * @param mode - unused
 * @param serial_fd - unused
 * @return int32_t - always returns 0
 */
int32_t serial_configure(const char *serial_port, Baudrate_t baudrate, Serial_mode_t mode, int *serial_fd);

/**
 * @brief Writes the data field onto the serial bus
 *
 * @param serial_fd - unused
 * @param data - pointer to the data
 * @param length - amount of bytes to send from the data buffer
 * @return int32_t - always returns 0
 */
int32_t serial_write(int serial_fd, char *data, size_t length);

/**
 * @brief copies message buffer into data buffer, and copies the value from
 * message pointer into length pointer.
 *
 *
 * @param serial_fd - unused
 * @param data - buffer to copy the data to
 * @param length - pointer to length that will be written to
 * @param timeout_milliseconds - sets the global timeout, so it can also be set by the interrupt
 * @return int32_t
 */
int32_t serial_read(int serial_fd, char *data, size_t *length, int timeout_milliseconds);

#endif
