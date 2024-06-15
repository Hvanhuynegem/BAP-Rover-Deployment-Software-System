/*
 * uart_communication.c file
 *
 * This is the UART communication library that contains the following functionality:
 *
 *  - starting, stopping, and resetting a timer
 *  - Initialising and configure the UART pins and states
 *  - Enter data into UART transmission buffer
 *  - UART interrupt handler
 *  - ISR for A1 UART module
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

#include "uart_communication.h"


/* UART A1 module initialisation and interrupt definitions. */
#define RX_BIT BIT6
#define TX_BIT BIT5
#define UCAxCTLW0 UCA1CTLW0
#define UCAxCTL1 UCA1CTL1
#define UCAxBR0_local UCA1BR0
#define UCAxBR1_local UCA1BR1
#define UCAxMCTLW UCA1MCTLW
#define UCAxIE UCA1IE
#define UCAxIFG UCA1IFG
#define UCAxTXBUF UCA1TXBUF
#define UCAxSTATW UCA1STATW

/* The amount of ticks that need to be reached for the timeout to trigger
 *   The tick rate is tied to SMCLK, which in current implementation is configured to ~16Mhz.
 *   The clock is further divided by 8 and only triggers a tick on the rising edge.
 *   The time for the timer to expire is calculated by  TIMEOUT_TICKS / (CLOCK_FREQUENCY / DIVIDER)
 *   Current timeout = 10000 / (16M / 8) = 10000 / 2M = 0.005s
 */
#define DEFAULT_TIMEOUT_TICKS 10000

/* global UART buffers and state variables */
uint8_t TX_buffer[UART_BUFFER_SIZE];
volatile uint16_t TX_start = 0;
volatile uint16_t TX_end = 0;

uint8_t RX_buffer[UART_BUFFER_SIZE];
volatile uint16_t RX_start = 0;
volatile uint16_t RX_end = 0;

/* Timeout variables */
volatile uint8_t timeout_active;
volatile int timeout;

/* size of incoming message */
volatile uint16_t message_pointer;
volatile uint16_t message_length;

/* uart state definition */
volatile UART_states UART_state;



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
void process_TX_buffer(void);

/*
 * Adds data to the TX_buffer array
 */
void add_to_TX_buffer(uint8_t byte);



inline static void start_timeout(void)
{
    // TACCR0 interrupt enabled
    TA0CCTL0 = CCIE;
    TA0CCR0 = timeout;
    // SMCLK, UP mode, divide the clock by 8
    TA0CTL = TASSEL__SMCLK | MC__UP | ID__8;
    timeout_active = 1;
}

inline static void reset_timeout(void)
{
    TA0CTL = MC__STOP | TACLR;

    // resets it again
    TA0CCTL0 = CCIE;
    TA0CCR0 = timeout;
    TA0CTL = TASSEL__SMCLK | MC__UP | ID__8;
}

inline static void stop_timeout(void)
{
    // Stops the timer
    TA0CTL = MC__STOP | TACLR;
    timeout_active = 0;
}

// Timer0_A0 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__((interrupt(TIMER0_A0_VECTOR))) Timer0_A0_ISR(void)
#else
#error Compiler not supported!
#endif
{
    UART_state = TIMEOUT;
    stop_timeout();
}


/**
 * Setup interrupts for A1 UART module to receive messages
 */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
#elif defined(__GNUC__)
void __attribute__((interrupt(USCI_A1_VECTOR))) USCI_A1_ISR(void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(UCA1IV, USCI_UART_UCTXCPTIFG))
    {
        case USCI_NONE:
            break;
        case USCI_UART_UCRXIFG:
        {

            uint8_t character = UCA1RXBUF;

            uart_interrupt_handler(character);

            __bic_SR_register_on_exit(LPM3_bits);    // Exits low power mode
        }
        break;
        case USCI_UART_UCTXIFG:
            process_TX_buffer();
            break;
        case USCI_UART_UCSTTIFG:
            break;
        case USCI_UART_UCTXCPTIFG:
            break;
        default:
            break;
    }
}

void uart_init(void)
{
    // Configure GPIO
    P2SEL1 |= RX_BIT | TX_BIT;       // Sets pins 2.6 and 2.5 to function in
    P2SEL0 &= ~(RX_BIT | TX_BIT);    // secondary mode (assumed to be UART)

    // Pins start in high-impedance mode on wake-up, this enables them again
    // and activates our port configurations
    PM5CTL0 &= ~LOCKLPM5;

    // Configure USCI_A1 for UART mode with 16Mhz clock and 115200 baud rate
    UCAxCTLW0 = UCSWRST;           // Put eUSCI needs to be in reset when modifying
                                   // configurations
    UCAxCTL1 |= UCSSEL__SMCLK;    // CLK = SMCLK, which currently is set to
                                   // 16Mhz (can be changed in future!)

    // User's Guide Table 30-5:
    // UCBRx = 8
    // UCOS16 = 1
    // UCBRFx = 10
    // UCBRSx = 0xF7
    UCAxBR0_local = 8;    // UCA1BR0 is int(16000000/(16*115200)) = 8
    UCAxBR1_local = 0x00;
    UCAxMCTLW |= UCOS16 | UCBRF_10 | 0xF700;    // UCOS16 enables over-sampling mode for high clock
                                                // speeds UCBRF_10 is int ((8.6805555555-8)*16) = 10
                                                // 0xF7 is the UCBRS modulation value

    UCAxCTL1 &= ~UCSWRST;    // Initialize eUSCI by clearing reset

    UCAxIE |= UCTXIE;    // Enable USCI_A1 TX
}

void uart_configure(void)
{
    UART_state = IDLE;
    timeout_active = 0;
    timeout = DEFAULT_TIMEOUT_TICKS;
    uart_init();
}

void uart_write(uint8_t *data, uint16_t length)
{
    uint16_t i;
    for (i = 0; i < length; i++) {
            add_to_TX_buffer(data[i]);
        }
    UCA1IE |= UCTXIE; // Enable TX interrupt
}

void add_to_TX_buffer(uint8_t byte) {
    TX_buffer[TX_end] = byte;
    TX_end = (TX_end + 1) % UART_BUFFER_SIZE;
}

inline static void uart_interrupt_handler(uint8_t character)
{
    // Put state in receiving mode if the state is idle and prepare for receiving a message
    if(UART_state == IDLE)
    {
        message_pointer = 0;
        message_length = 0;
        // Check if its a valid start of the message, otherwise, do nothing
        if(character == MSG_START_BYTE)
        {
            UART_state = RECEIVING;
            start_timeout();
        }
    }
    // Check if state is in receiving mode to receive messages
    if(UART_state == RECEIVING)
    {

        // If timer is active, reset it, to not cause a timeout
        if(timeout_active == (uint8_t)1)
        {
            reset_timeout();
        }

        if(message_pointer >= UART_BUFFER_SIZE)
        {
            UART_state = BUFFER_FULL;
        }
        else
        {
            // First 3 bytes should be MSG_START_BYTE, MSG_TYPE and LENGTH according to Message protocol format.
            if(message_pointer < (uint16_t)3)
            {
                RX_buffer[RX_end] = character;
                RX_end = (RX_end + 1) % UART_BUFFER_SIZE;
                message_pointer++;
                // save the size of the message
                if(message_pointer == (uint16_t)2)
                {
                    // slip encoding start byte + start byte + msg type size + length byte + payload size +
                    // checksum size + end_byte size + slip encoding byte size
                    const uint16_t seven = 7;
                    message_length = seven + character;
                }
                else {}
            }
            else
            {
                // Once the first bytes have been received, rest can be copied according to the length
                if(message_pointer < (message_length - (uint16_t)1))
                {
                    RX_buffer[RX_end] = character;
                    RX_end = (RX_end + 1) % UART_BUFFER_SIZE;
                    message_pointer++;
                }
                // If this is the last byte, set state to PACKET_RECEIVED
                else
                {
                    RX_buffer[RX_end] = character;
                    RX_end = (RX_end + 1) % UART_BUFFER_SIZE;
                    message_pointer++;
                    stop_timeout();
                    UART_state = RECEIVED;

                }
            }
        }
    }

}

inline void process_TX_buffer(void)
{
    if (TX_start != TX_end) {
        // if the indices of the Transmission buffer are not equal
        //then put the next character to be sent into the UCA1TXBUF register
        UCA1TXBUF = TX_buffer[TX_start];
        // use circular indexing for the buffer
        TX_start = (TX_start + 1) % UART_BUFFER_SIZE;
    } else {
        UCA1IE &= ~UCTXIE; // Disable TX interrupt if buffer is empty
    }
}

uint16_t read_RX_buffer(uint16_t start, uint16_t end){
    // Process received data
    uint16_t input_length = (end >= start) ? (end - start) : (UART_BUFFER_SIZE - start + end);
    uint8_t decoded_msg[UART_BUFFER_SIZE];
    uint16_t decoded_length;

    // Temporary buffer to hold the data from the circular buffer
    uint8_t temp_buffer[UART_BUFFER_SIZE];
    uint16_t temp_length = 0;

    // Copy data from the circular buffer to the temporary buffer
    if (end >= start) {
        memcpy(temp_buffer, &RX_buffer[start], input_length);
        temp_length = input_length;
    } else {
        uint16_t first_part_length = UART_BUFFER_SIZE - start;
        memcpy(temp_buffer, &RX_buffer[start], first_part_length);
        memcpy(temp_buffer + first_part_length, RX_buffer, end);
        temp_length = first_part_length + end;
    }

    // Decode the SLIP-encoded message
    slip_decode(temp_buffer, temp_length, decoded_msg, &decoded_length);

    // Deserialize the message
    Message* msg_pointer = convert_array_to_message(decoded_msg, decoded_length);

    // Handle the message
    handle_message(msg_pointer);

    // Clean up the dynamically allocated message
    delete msg_pointer;

    // reset the start index
    start = end;

    // return the updated start index
    return start;
}

void setup_SMCLK(void)
{
    // Set DCO frequency to 16 MHz
    CSCTL0_H = CSKEY >> 8;           // Unlock CS registers
    CSCTL1 = DCOFSEL_4 | DCORSEL;    // Set DCO to 16 MHz (DCORSEL selects high range, DCOFSEL_4 sets it to 16 MHz)
    CSCTL2 = SELS__DCOCLK;           // Set SMCLK source to DCO
    CSCTL3 = DIVS__1;                // Set SMCLK divider to 1 (16 MHz)

    CSCTL0_H = 0;                    // Lock CS registers
}
