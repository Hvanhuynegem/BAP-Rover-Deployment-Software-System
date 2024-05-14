#include "communication/landerComm.h"
#include <msp430.h>
#include <stdint.h>

void initialize_UART(int baudrate);
void slip_encode(uint8_t *buffer, uint16_t length);
void slip_decode(uint8_t *buffer, uint16_t *received_length);
void uart_send_byte(uint8_t byte);
uint8_t uart_receive_byte();

void initialize_UART(int baudrate){
    // Assuming SMCLK has been configured to 16MHz, calculate BRW for 9600 baud
    UCA0CTL1 |= UCSWRST;        // Put eUSCI in reset
    UCA0CTL1 |= UCSSEL__SMCLK;  // SMCLK

    // Configure baud rate
    UCA0BR0 = 104;              // 9600 baud if SMCLK = 16MHz
    UCA0BR1 = 0;
    UCA0MCTLW = 0x5551; // Modulation UCBRSx=0, UCBRFx=1, oversampling

    UCA0CTL1 &= ~UCSWRST;       // Initialize eUSCI
    UCA0IE |= UCRXIE;           // Enable USCI_A0 RX interrupt
};

void slip_encode(uint8_t *buffer, uint16_t length){
    const uint8_t END = 0xC0;     // End of packet character
    const uint8_t ESC = 0xDB;     // Escape character
    const uint8_t ESC_END = 0xDC; // Escaped substitution for the END data byte
    const uint8_t ESC_ESC = 0xDD; // Escaped substitution for the ESC data byte

    uart_send_byte(END); // Send initial END character to flush any data

    for (uint16_t i = 0; i < length; i++) {
        switch (buffer[i]) {
            case END:
                uart_send_byte(ESC);
                uart_send_byte(ESC_END);
                break;
            case ESC:
                uart_send_byte(ESC);
                uart_send_byte(ESC_ESC);
                break;
            default:
                uart_send_byte(buffer[i]);
        }
    }

    uart_send_byte(END); // End the packet
};

void slip_decode(uint8_t *buffer, uint16_t *received_length){
    const uint8_t END = 0xC0;     // End of packet character
        const uint8_t ESC = 0xDB;     // Escape character
        const uint8_t ESC_END = 0xDC; // Escaped substitution for the END data byte
        const uint8_t ESC_ESC = 0xDD; // Escaped substitution for the ESC data byte

        uint8_t c;
        bool is_escaped = false;
        *received_length = 0;

        while ((c = uart_receive_byte())) {
            if (c == END) {
                if (*received_length) // If there's meaningful data, return
                    return;
                else // If no data, keep looking for start of next packet
                    continue;
            }

            if (is_escaped) {
                if (c == ESC_END) c = END;
                else if (c == ESC_ESC) c = ESC;
                is_escaped = false;
            } else if (c == ESC) {
                is_escaped = true;
                continue;
            }

            buffer[(*received_length)++] = c;
        }
};

void uart_send_byte(uint8_t byte){
    while (!(UCA0IFG & UCTXIFG)); // Wait for the transmit buffer to be ready
        UCA0TXBUF = byte;
};

uint8_t uart_receive_byte(){
    while (!(UCA0IFG & UCRXIFG)); // Wait for a receive buffer to be ready
        return UCA0RXBUF;
};
