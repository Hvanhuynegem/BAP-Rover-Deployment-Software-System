#include "landerComm.h"
#include "protocol.h"
#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>


void initialize_clock(void){
    PJSEL0 |= BIT4 | BIT5;                  // Configure XT1 pins

    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode to activate previously configured port settings

    // XT1 Setup
    CSCTL0_H = CSKEY >> 8;                    // Unlock CS registers
    CSCTL1 = DCOFSEL_0;                       // Set DCO to 1MHz
    CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK;
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;     // Set all dividers
    CSCTL4 &= ~LFXTOFF;                       // Enable LFXT1
    do
    {
        CSCTL5 &= ~LFXTOFFG;                    // Clear XT1 fault flag
        SFRIFG1 &= ~OFIFG;
    } while (SFRIFG1 & OFIFG);                // Test oscillator fault flag
    CSCTL0_H = 0;                             // Lock CS registers
}

void initialize_UART_A0(void) {
    PM5CTL0 &= ~LOCKLPM5; // Disable GPIO power-on default high-impedance mode

    P2SEL1 |= BIT0 | BIT1; // Set pins 2.0 and 2.1 for UART
    P2SEL0 &= ~(BIT0 | BIT1);

    UCA0CTL1 |= UCSWRST;  // Put eUSCI in reset
    UCA0CTL1 = UCSSEL__ACLK; // Select ACLK as the clock source
    UCA0BR0 = 3;           // Set baud rate to 9600
    UCA0MCTLW |= 0x5300;   // Modulation settings for 9600 baud
    UCA0BR1 = 0;
    // enable transmission and receiving interrupts.
    UCA1IE |= UCRXIE;
    UCA1IE |= UCTXIE;
    UCA0CTL1 &= ~UCSWRST;  // Initialize eUSCI (release from reset)
}

void initialize_UART_A1(void) {
    // Configure GPIO for UART
    P2SEL1 |= BIT6 | BIT5;        // Sets pins 2.5 and 2.6 to function in
    P2SEL0 &= ~(BIT6 | BIT5);     // secondary mode (assumed to be UART)

    PM5CTL0 &= ~LOCKLPM5;             // Disable the GPIO power-on default high-impedance mode to activate previously configured port settings

    // Configure USCI_A1 for UART mode
    UCA1CTL1 |= UCSWRST;              // Put eUSCI in reset
    UCA1CTL1 = UCSSEL__ACLK;          // Select ACLK as the clock source
    UCA1BR0 = 3;                      // Set baud rate to 9600
    UCA1MCTLW |= 0x5300;              // Modulation settings for 9600 baud
    UCA1BR1 = 0;
    // enable transmission and receiving interrupts.
    UCA1IE |= UCRXIE;
    UCA1IE |= UCTXIE;
    UCA1CTL1 &= ~UCSWRST;             // Initialize eUSCI (release from reset)
}

void slip_encode(const uint8_t *buffer, uint16_t length) {
    const uint8_t END = 0xC0;
    const uint8_t ESC = 0xDB;
    const uint8_t ESC_END = 0xDC;
    const uint8_t ESC_ESC = 0xDD;

    while (!(UCA1IFG & UCTXIFG));
    UCA1TXBUF = END;

    for (uint16_t i = 0; i < length; i++) {
        switch (buffer[i]) {
            case END:
                while (!(UCA1IFG & UCTXIFG));
                UCA1TXBUF = ESC;
                while (!(UCA1IFG & UCTXIFG));
                UCA1TXBUF = ESC_END;
                break;
            case ESC:
                while (!(UCA1IFG & UCTXIFG));
                UCA1TXBUF = ESC;
                while (!(UCA1IFG & UCTXIFG));
                UCA1TXBUF = ESC_ESC;
                break;
            default:
                while (!(UCA1IFG & UCTXIFG));
                UCA1TXBUF = buffer[i];
        }
    }

    while (!(UCA1IFG & UCTXIFG));
    UCA1TXBUF = END;
}

void slip_decode(uint8_t *buffer, uint16_t *received_length) {
    const uint8_t END = 0xC0;
    const uint8_t ESC = 0xDB;
    const uint8_t ESC_END = 0xDC;
    const uint8_t ESC_ESC = 0xDD;

    uint8_t c;
    bool is_escaped = false;
    *received_length = 0;

    while (1) {
        while (!(UCA1IFG & UCRXIFG)); // Wait for data
        c = UCA1RXBUF;

        if (c == END) {
            if (*received_length) {
                return;
            } else {
                continue;
            }
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
}

void start_timer(void) {
    TA0CCR0 = 32767; // Set timer count for approximately 1 second (32.768 kHz ACLK)
    TA0CCTL0 |= CCIE; // Enable timer interrupt
    TA0CTL = TASSEL_1 | MC_1; // ACLK, up mode
}

bool send_message_with_ack(const uint8_t *message, uint16_t length) {
    slip_encode(message, length);

    uint8_t ack[3];
    uint16_t received_length;
    slip_decode(ack, &received_length);

    return (received_length == 3 && ack[0] == 'A' && ack[1] == 'C' && ack[2] == 'K');
}

void send_data(const uint8_t *data, uint16_t length) {
    slip_encode(data, length);
}

void handle_incoming_data(uint8_t *buffer, uint16_t length) {
    Message msg;
    memcpy(&msg, buffer, length);
    handle_message(&msg);
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A(void) {
    __bic_SR_register_on_exit(LPM3_bits); // Exit low power mode on interrupt exit
}
