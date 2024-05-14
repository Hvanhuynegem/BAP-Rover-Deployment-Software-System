#include <msp430.h>
#include <stdint.h>

/*
 * This function initialises the UART ports using a baud rate.
 *
 * parameters:
 *  int baudrate - the rate at which data is sent in baud/s
 */
void initialize_UART(int baudrate);

/*
* This function encodes an array and sends it through the two UART pins.
*
* parameters
*   uint8_t *buffer: message to be sent
*   uint16_t length: length of packet to be sent
*/
void slip_encode(uint8_t *buffer, uint16_t length);

/*
* This function decodes an array and receives it through the two UART pins.
*
* parameters
*   uint8_t *buffer: message to be sent
*   uint16_t *received_length: received length of received packet
*/
void slip_decode(uint8_t *buffer, uint16_t *received_length);

/*
 * This function sends a byte via the UART TX port.
 *
 * parameters
 *  uint8_t byte : the to be transmitted byte
 */
void uart_send_byte(uint8_t byte);

/*
 * This function receives a byte via the UART RX port.
 *
 * returns
 *  uint8_t byte : the received byte
 */
uint8_t uart_receive_byte();
