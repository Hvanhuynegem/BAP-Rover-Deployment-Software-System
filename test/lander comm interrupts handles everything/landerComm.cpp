/*
 * LanderComm.cpp file
 *
 * This file will include all functionality regarding communication with the lander. The SLIP protocol is being used as communication protocol.
 * Within this communication protocol we designed another communication protocol that makes it easier to parse received messages such that the handler can handle data more effectively.
 *
 * Author: Henri Vanhuynegem
 * Date: 23/05/2024
 *
 */

#include "communication/landerComm.h"

uint8_t TX_buffer[UART_BUFFER_SIZE];
volatile uint16_t TX_start = 0;
volatile uint16_t TX_end = 0;

uint8_t RX_buffer[UART_BUFFER_SIZE];
volatile uint16_t RX_start = 0;
volatile uint16_t RX_end = 0;
bool receiving_message = false;

void initialize_UART_A1_115200(void) {
    // Configure GPIO for UART
    P2SEL1 |= BIT6 | BIT5;        // Sets pins 2.5 and 2.6 to function in
    P2SEL0 &= ~(BIT6 | BIT5);     // secondary mode (assumed to be UART)

    PM5CTL0 &= ~LOCKLPM5;             // Disable the GPIO power-on default high-impedance mode to activate previously configured port settings

    // Configure USCI_A1 for UART mode
      UCA1CTLW0 = UCSWRST;                      // Put eUSCI in reset
      UCA1CTL1 |= UCSSEL__SMCLK;                // CLK = SMCLK
      UCA1BR0 = 8;                              // 1000000/115200 = 8.68
      UCA1MCTLW = 0xD600;                       // 1000000/115200 - INT(1000000/115200)=0.68
                                                // UCBRSx value = 0xD6 (See UG)
      UCA1BR1 = 0;
      UCA1CTL1 &= ~UCSWRST;                     // release from reset
      UCA1IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt
}


void initialize_UART_A1_9600(void) {
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
    UCA1CTL1 &= ~UCSWRST;                     // release from reset
    UCA1IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt
}

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

void serialize_message(const Message* msg, uint8_t* buffer, uint16_t* length) {
    uint16_t index = 0;

    // Copy start_byte
    buffer[index++] = msg->start_byte;

    // Copy msg_type
    buffer[index++] = msg->msg_type;

    // Copy length
    buffer[index++] = msg->length;

    // Copy payload
    if (!msg->payload.empty()) {
        memcpy(&buffer[index], msg->payload.data(), msg->payload.size());
        index += msg->payload.size();
    }

    // Copy checksum
    buffer[index++] = msg->checksum;

    // Copy end_byte
    buffer[index++] = msg->end_byte;

    // Set the total length of the serialized message
    *length = index;
}


Message* deserialize_message(const uint8_t* buffer, uint16_t length) {
    Message* msg = new Message();
    uint16_t index = 0;

    // Extract start_byte
    msg->start_byte = buffer[index++];

    // Extract msg_type
    msg->msg_type = buffer[index++];

    // Extract length
    msg->length = buffer[index++];

    // Extract payload
    msg->payload.clear();
    if (msg->length > 0) {
        msg->payload.insert(msg->payload.end(), &buffer[index], &buffer[index] + msg->length);
        index += msg->length;
    }

    // Extract checksum
    msg->checksum = buffer[index++];

    // Extract end_byte
    msg->end_byte = buffer[index++];

    return msg;
}


void slip_encode(const uint8_t *buffer, uint16_t length, uint8_t *output_buffer, uint16_t *received_length) {
    const uint8_t END = 0xC0;
    const uint8_t ESC = 0xDB;
    const uint8_t ESC_END = 0xDC;
    const uint8_t ESC_ESC = 0xDD;

    uint16_t index = 0;

    output_buffer[index++] = END;

    for (uint16_t i = 0; i < length; i++) {
        switch (buffer[i]) {
            case END:
                output_buffer[index++] = ESC;
                output_buffer[index++] = ESC_END;
                break;
            case ESC:
                output_buffer[index++] = ESC;
                output_buffer[index++] = ESC_ESC;
                break;
            default:
                output_buffer[index++] = buffer[i];
        }
    }

    output_buffer[index++] = END;

    *received_length = index;
}


void slip_decode(const uint8_t *input_buffer, uint16_t input_length, uint8_t *output_buffer, uint16_t *output_length) {
    const uint8_t END = 0xC0;
    const uint8_t ESC = 0xDB;
    const uint8_t ESC_END = 0xDC;
    const uint8_t ESC_ESC = 0xDD;

    bool is_escaped = false;
    *output_length = 0;

    for (uint16_t i = 0; i < input_length; i++) {
        uint8_t c = input_buffer[i];

        if (c == END) {
            if (*output_length > 0) {
                // End of packet
                return;
            } else {
                // Start of packet, skip
                continue;
            }
        }

        if (is_escaped) {
            if (c == ESC_END) {
                c = END;
            } else if (c == ESC_ESC) {
                c = ESC;
            }
            is_escaped = false;
        } else if (c == ESC) {
            is_escaped = true;
            continue;
        }

        output_buffer[(*output_length)++] = c;
    }
}


void send_message(const Message* msg) {
    uint8_t serialized_msg[UART_BUFFER_SIZE];
    uint16_t serialized_length;

    // Serialize the message
    serialize_message(msg, serialized_msg, &serialized_length);

    uint8_t encoded_msg[UART_BUFFER_SIZE];
    uint16_t encoded_length;

    // Encode the message using SLIP
    slip_encode(serialized_msg, serialized_length, encoded_msg, &encoded_length);

    // Send the encoded message
    uart_send_data(encoded_msg, encoded_length);
}

void uart_send_data(const uint8_t* data, uint16_t length) {
    for (uint16_t i = 0; i < length; i++) {
        add_to_TX_buffer(data[i]);
    }
    process_TX_buffer();
}

void add_to_TX_buffer(uint8_t byte) {
    TX_buffer[TX_end] = byte;
    TX_end = (TX_end + 1) % UART_BUFFER_SIZE;
    UCA1IE |= UCTXIE; // Enable TX interrupt
}

void process_TX_buffer(void) {
    if (TX_start != TX_end) {
        UCA1TXBUF = TX_buffer[TX_start];
        TX_start = (TX_start + 1) % UART_BUFFER_SIZE;
    } else {
        UCA1IE &= ~UCTXIE; // Disable TX interrupt if buffer is empty
    }
}

// Interrupt service routine for UART
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void) {
    switch (__even_in_range(UCA1IV, 4)) {
        case 0: break;        // Vector 0 - no interrupt
        case 2:               // Vector 2 - RXIFG
            {
                uint8_t received_byte = UCA1RXBUF;
                RX_buffer[RX_end] = received_byte;
                RX_end = (RX_end + 1) % UART_BUFFER_SIZE;

                if (received_byte == 0xC0) {
                    if (!receiving_message) {
                        // Start of a new message
                        receiving_message = true;
                        RX_start = RX_end - 1;
                    } else if (((RX_end - 1) % UART_BUFFER_SIZE != RX_start)) {
                        // End of a message
                        receiving_message = false;

                        // Process received data
                        uint8_t decoded_msg[UART_BUFFER_SIZE];
                        uint16_t input_length = (RX_end >= RX_start) ? (RX_end - RX_start) : (UART_BUFFER_SIZE - RX_start + RX_end);
                        uint16_t decoded_length;

                        slip_decode(RX_buffer, input_length, decoded_msg, &decoded_length);

//                        Message init_msg = create_message(MSG_TYPE_RESPONSE, decoded_msg, decoded_length);
//
//                        // Send the initialization message
//                        send_message(&init_msg);

                        // Deserialize the message
                        Message* msg_pointer = deserialize_message(decoded_msg, decoded_length);

                        // Handle the message
                        handle_message(msg_pointer);

                        // Clean up the dynamically allocated message
                        delete msg_pointer;

                        RX_start = RX_end = 0;  // Reset RX buffer
                    }
                }
            }
            break;
        case 4:               // Vector 4 - TXIFG
            process_TX_buffer();
            break;
        default: break;
    }
}
