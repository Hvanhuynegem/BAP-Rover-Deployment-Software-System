//#include "communication/landerComm.h"
//#include <msp430.h>
//#include <stdint.h>
//
//#define TX_BUFFER_SIZE 256   // Reduced buffer size
//#define RX_BUFFER_SIZE 256   // Reduced buffer size
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
//    // Configure baud rate for 9600 with 1MHz clock
//    UCA0BR0 = 6;                // Baud rate setting
//    UCA0BR1 = 0;                // Baud Rate Control
//    UCA0MCTLW = 0x2081;         // Modulation UCBRSx=0x20, UCBRFx=8, oversampling
//
//    UCA0CTL1 &= ~UCSWRST;       // Initialize eUSCI
//    UCA0IE |= UCRXIE;           // Enable USCI_A0 RX interrupt
//    UCA0IE |= UCTXIE;           // Enable USCI_A0 TX interrupt
//}
//
//// SLIP encoding function
//void slip_encode(uint8_t *buffer, uint16_t length) {
//    const uint8_t END = 0xC0;
//    const uint8_t ESC = 0xDB;
//    const uint8_t ESC_END = 0xDC;
//    const uint8_t ESC_ESC = 0xDD;
//
//    tx_buffer_add_byte(END);  // Start with END to flush any previous data
//
//    for (uint16_t i = 0; i < length; i++) {
//        switch (buffer[i]) {
//            case END:
//                tx_buffer_add_byte(ESC);
//                tx_buffer_add_byte(ESC_END);
//                break;
//            case ESC:
//                tx_buffer_add_byte(ESC);
//                tx_buffer_add_byte(ESC_ESC);
//                break;
//            default:
//                tx_buffer_add_byte(buffer[i]);
//        }
//    }
//
//    tx_buffer_add_byte(END);  // End with END to mark packet completion
//    UCA0IE |= UCTXIE;         // Enable transmit interrupt
//}
//
//// SLIP decoding function
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
//    for (uint16_t i = 0; i < rx_index; i++) {
//        c = rx_buffer[i];
//
//        if (c == END) {
//            if (*received_length) {  // If there's meaningful data, clear buffer and return
//                rx_index = 0;  // Clear the buffer index for new data
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
//
//    rx_index = 0;  // Reset buffer index after processing
//}
//
//
//// ISR for receiving and transmitting UART data
//#pragma vector=USCI_A0_VECTOR
//__interrupt void USCI_A0_ISR(void) {
//    switch(__even_in_range(UCA0IV, USCI_UART_UCTXCPTIFG)) {
//        case USCI_NONE: break;
//        case USCI_UART_UCRXIFG: // receive interrupt
//            rx_buffer[rx_index++] = UCA0RXBUF;  // Read RX buffer and increment index
//            if (rx_index >= RX_BUFFER_SIZE) {
//                rx_index = 0;  // Reset index if buffer is full
//            }
//            break;
//        case USCI_UART_UCTXIFG: // transmit interrupt
//            if (tx_out_index != tx_in_index) {
//                UCA0TXBUF = tx_buffer[tx_out_index++];
//                if (tx_out_index >= TX_BUFFER_SIZE) tx_out_index = 0;  // Circular buffer
//            } else {
//                UCA0IE &= ~UCTXIE;  // Disable TX interrupt if no more data to send
//            }
//            break;
//        default: break;
//    }
//}