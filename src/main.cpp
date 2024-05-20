#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

#define TX_BIT BIT5  // Define TXD as P2.5
#define RX_BIT BIT6  // Define RXD as P2.6

#define TX_BUFFER_SIZE 256
#define RX_BUFFER_SIZE 256

volatile uint8_t tx_buffer[TX_BUFFER_SIZE];
volatile uint16_t tx_in_index = 0;  // Index where data is added to the buffer
volatile uint16_t tx_out_index = 0; // Index where data is sent from the buffer

volatile uint8_t rx_buffer[RX_BUFFER_SIZE];
volatile uint16_t rx_index = 0;     // Index for receiving data

void tx_buffer_add_byte(uint8_t byte);
void initialize_UART(void);
void slip_encode(const uint8_t *buffer, uint16_t length);
void slip_decode(uint8_t *buffer, uint16_t *received_length);
void initialize_clock(void);
void start_timer(void);


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


void initialize_UART(void){
    // Configure GPIO for UART
    P2SEL1 |= RX_BIT | TX_BIT;        // Sets pins 2.5 and 2.6 to function in
    P2SEL0 &= ~(RX_BIT | TX_BIT);     // secondary mode (assumed to be UART)

    PM5CTL0 &= ~LOCKLPM5;             // Disable the GPIO power-on default high-impedance mode to activate previously configured port settings

    // Configure USCI_A1 for UART mode
    UCA1CTL1 |= UCSWRST;              // Put eUSCI in reset
    UCA1CTL1 = UCSSEL__ACLK;          // Select ACLK as the clock source
    UCA1BR0 = 3;                      // Set baud rate to 9600
    UCA1MCTLW |= 0x5300;              // Modulation settings for 9600 baud
    UCA1BR1 = 0;
    UCA1CTL1 &= ~UCSWRST;             // Initialize eUSCI (release from reset)
}

void tx_buffer_add_byte(uint8_t byte) {
    tx_buffer[tx_in_index++] = byte;
    if (tx_in_index >= TX_BUFFER_SIZE) tx_in_index = 0; // Circular buffer

    // Enable TX interrupt to start sending data
    UCA1IE |= UCTXIE;
}


void slip_encode(const uint8_t *buffer, uint16_t length) {
    const uint8_t END = 0xC0;
    const uint8_t ESC = 0xDB;
    const uint8_t ESC_END = 0xDC;
    const uint8_t ESC_ESC = 0xDD;

    // Add initial END character to flush any previous data
    tx_buffer_add_byte(END);

    for (uint16_t i = 0; i < length; i++) {
        switch (buffer[i]) {
            case END:
                tx_buffer_add_byte(ESC);
                tx_buffer_add_byte(ESC_END);
                break;
            case ESC:
                tx_buffer_add_byte(ESC);
                tx_buffer_add_byte(ESC_ESC);
                break;
            default:
                tx_buffer_add_byte(buffer[i]);
        }
    }

    // Add final END character to mark packet completion
    tx_buffer_add_byte(END);
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
        while (!(UCA1IFG & UCRXIFG));  // Wait for data
        c = UCA1RXBUF;

        if (c == END) {
            if (*received_length) {  // If there's meaningful data, return
                return;
            } else {  // Continue if packet is empty (just an END received)
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
    TA0CCR0 = 32767;          // Set timer count for approximately 1 second (assuming 32.768 kHz ACLK)
    TA0CCTL0 |= CCIE;         // Enable timer interrupt
    TA0CTL = TASSEL_1 | MC_1; // ACLK, up mode
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A(void) {
    __bic_SR_register_on_exit(LPM3_bits); // Exit low power mode on interrupt exit
}


int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;  // Stop watchdog timer

    initialize_clock();
    initialize_UART();
    start_timer();

    __bis_SR_register(GIE);  // Enable global interrupts

    while (1) {
        // Enter low power mode and wait for RX interrupt
        __bis_SR_register(LPM0_bits + GIE); // Enter LPM0 and wait for RX interrupt

        // Decode received data
        uint16_t received_length;
        slip_decode((uint8_t*)rx_buffer, &received_length);

        // Encode and retransmit received data
        slip_encode((const uint8_t*)rx_buffer, received_length);
    }
}


#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_A1_VECTOR))) USCI_A1_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch (__even_in_range(UCA1IV, 18)) {
        case 0x00: break;  // Vector 0: No interrupts
        case 0x02:         // Vector 2: UCRXIFG - The UCRXIFG interrupt flag is set each time a character is received and loaded into UCAxRXBUF.
            // Store received byte in RX buffer
            rx_buffer[rx_index++] = UCA1RXBUF;
            if (rx_index >= RX_BUFFER_SIZE) rx_index = 0;  // Circular buffer

            // Exit low power mode to process received data
            __bic_SR_register_on_exit(LPM0_bits);
            break;
        case 0x04:        // Vector 4: UCTXIFG
            if (tx_out_index != tx_in_index) {
                UCA1TXBUF = tx_buffer[tx_out_index++];
                if (tx_out_index >= TX_BUFFER_SIZE) tx_out_index = 0; // Circular buffer
            } else {
                UCA1IE &= ~UCTXIE; // Disable TX interrupt when done
            }
            break;
        case 0x06:  // Vector 6: UCSTTIFG: START byte received interrupt. This flag is set when the UART module receives a START byte. This flag can be cleared by writing 0 to it.
            UCA1IFG &= ~UCSTTIFG;
            break;
        case 0x08:  // Vector 8: UCTXCPTIFG: Transmit complete interrupt. This flag is set after the complete UART byte in the internal shift register including STOP bit is shifted out. This flag can be cleared by writing 0 to it.
            UCA1IFG &= ~UCTXCPTIFG;
            break;
        default: break;
    }
}

