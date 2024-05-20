//#include "communication/landerComm.h"
//#include <msp430.h>
//#include <stdint.h>
//
//#define TX_BUFFER_SIZE 256   // Reduced buffer size
//#define RX_BUFFER_SIZE 256   // Reduced buffer size
//#define RX_BIT BIT6
//#define TX_BIT BIT5
//
//volatile uint8_t tx_buffer[TX_BUFFER_SIZE];
//volatile uint16_t tx_in_index = 0;  // Index where data is added to the buffer
//volatile uint16_t tx_out_index = 0; // Index where data is sent from the buffer
//
//volatile uint8_t rx_buffer[RX_BUFFER_SIZE];
//volatile uint16_t rx_index = 0;     // Index for receiving data
//
//// Function to add data to the transmit buffer
//void tx_buffer_add_byte(uint8_t byte) {
//    tx_buffer[tx_in_index++] = byte;
//    if (tx_in_index >= TX_BUFFER_SIZE) tx_in_index = 0; // Circular buffer
//}
//
//// UART initialization function
//void initialize_UART(int baudrate) {
//    UCA0CTLW0_L |= UCSWRST;        // Put eUSCI in reset
//    UCA0CTL1 |= UCSSEL__SMCLK;  // Select SMCLK
//
//    // Configure GPIO
//    P2SEL1 |= RX_BIT | TX_BIT;       // Sets pins 2.1 and 2.0 to function in
//    P2SEL0 &= ~(RX_BIT | TX_BIT);    // secondary mode (assumed to be UART)
//
//    // Configure baud rate for 9600 with 1MHz clock
//    UCA0BR0 = 6;                // Baud rate setting
//    UCA0BR1 = 0;                // Baud Rate Control
//    UCA0MCTLW = 0x2081;         // Modulation UCBRSx=0x20, UCBRFx=8, oversampling
//
//    UCA0CTL1 &= ~UCSWRST;       // Initialize eUSCI
//}
//
//// SLIP encoding function with polling
//void slip_encode(uint8_t *buffer, uint16_t length) {
//    const uint8_t END = 0xC0;
//    const uint8_t ESC = 0xDB;
//    const uint8_t ESC_END = 0xDC;
//    const uint8_t ESC_ESC = 0xDD;
//
//    // Send initial END character to flush any previous data
//    while (!(UCA0IFG & UCTXIFG));
//    UCA0TXBUF = END;
//
//    for (uint16_t i = 0; i < length; i++) {
//        switch (buffer[i]) {
//            case END:
//                while (!(UCA0IFG & UCTXIFG));
//                UCA0TXBUF = ESC;
//                while (!(UCA0IFG & UCTXIFG));
//                UCA0TXBUF = ESC_END;
//                break;
//            case ESC:
//                while (!(UCA0IFG & UCTXIFG));
//                UCA0TXBUF = ESC;
//                while (!(UCA0IFG & UCTXIFG));
//                UCA0TXBUF = ESC_ESC;
//                break;
//            default:
//                while (!(UCA0IFG & UCTXIFG));
//                UCA0TXBUF = buffer[i];
//        }
//    }
//
//    // Send final END character to mark packet completion
//    while (!(UCA0IFG & UCTXIFG));
//    UCA0TXBUF = END;
//}
//
//// SLIP decoding function with polling
//void slip_decode(uint8_t *buffer, uint16_t *received_length) {
//    const uint8_t END = 0xC0;
//    const uint8_t ESC = 0xDB;
//    const uint8_t ESC_END = 0xDC;
//    const uint8_t ESC_ESC = 0xDD;
//
//    uint8_t c;
//    bool is_escaped = false;
//    *received_length = 0;
//
//    while (1) {
//        while (!(UCA0IFG & UCRXIFG));  // Wait for data
//        c = UCA0RXBUF;
//
//        if (c == END) {
//            if (*received_length) {  // If there's meaningful data, return
//                return;
//            } else {  // Continue if packet is empty (just an END received)
//                continue;
//            }
//        }
//
//        if (is_escaped) {
//            if (c == ESC_END) c = END;
//            else if (c == ESC_ESC) c = ESC;
//            is_escaped = false;
//        } else if (c == ESC) {
//            is_escaped = true;
//            continue;
//        }
//
//        buffer[(*received_length)++] = c;
//    }
//}
