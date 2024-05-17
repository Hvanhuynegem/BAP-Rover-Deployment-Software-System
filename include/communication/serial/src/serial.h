/**
 * Author(s):
 *    Marco van Eerden
 *    Youri Klaassens
 *
 * This is the serial library for the OBC. It contains code for:
 * - Configuring the serial bus.
 * - Reading from the serial bus.
 * - Writing to the serial bus.
 * - Closing the serial bus.
 */
#ifndef _SERIAL_H
#define _SERIAL_H

#include <stddef.h>
#include <stdint.h>

/* All buses that we use on the OBC. */
// TODO: figure out which tty device we will use on the actual OBC.
#define BUS0 "/dev/ttyUSB0"
#define BUS1 "/dev/ttyUSB1"
#define BUS2 "/dev/ttyUSB2"
#define BUS3 "/dev/ttyUSB3"
#define BUS4 "/dev/ttyUSB4"

/* List of valid baud rates. */
typedef enum
{
    BAUD9600,
    BAUD115200
} Baudrate_t;

/* List of serial modes. */
typedef enum
{
    SERIAL_BLOCKING,
    SERIAL_NONBLOCKING
} Serial_mode_t;

/**
 * Closes the serial file descriptor.
 *
 * @param serial_fd Serial file descriptor.
 *
 * @return Error code from close().
 */
uint32_t serial_close(int serial_fd);

/**
 * Configures the serial bus according to selected baud rate and mode.
 *
 * @param serial_port TTY serial port.
 * @param baudrate    Serial baud rate.
 * @param mode        Serial mode (BLOCKING or NON_BLOCKING).
 * @param serial_fd   File descriptor is written to this pointer.
 *
 * @return -1 if file descriptor is invalid
 *         -2 if we could not get termios attributes
 *         -3 if an invalid baud rate was selected
 *         -4 if setting termios attributes failed
 *         -5 if setting RS485 attributes failed
 *          0 otherwise
 */
uint32_t serial_configure(const char *serial_port, Baudrate_t baudrate, Serial_mode_t mode, int *serial_fd);

/**
 * Writes the given data to the serial bus.
 *
 * @param serial_fd File descriptor for the serial bus.
 * @param data      Data to write.
 * @param length    Length of the data in bytes.
 *
 * @return 1 if a write error occurred
 *         2 if the number of bytes written was not equal to the length given
 *         0 otherwise
 */
uint32_t serial_write(int serial_fd, char *data, size_t length);

/**
 * Reads data from the serial bus.
 *
 * @param serial_fd            File descriptor for the serial bus.
 * @param data                 Received data is written to this pointer.
 * @param length               The length of the received data is written to this pointer.
 * @param timeout_milliseconds Number of milliseconds after which a timeout is signalled.
 *
 * @return 1 if a read error occurred
 *         3 if the preamble of the received message is invalid
 *         6 if the destination of the received message is invalid
 *         0 otherwise
 */
uint32_t serial_read(int serial_fd, char *data, size_t *length, uint32_t timeout_milliseconds);

#endif /* _SERIAL_H */
