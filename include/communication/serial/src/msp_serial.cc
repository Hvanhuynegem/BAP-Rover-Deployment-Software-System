#include "driverlib.h"
#include <msp_serial.h>
#include <string.h>

/* Message reading variables */
volatile uint16_t BLE_message_length;

/* Timeout variables */
volatile uint8_t timeout_active;
volatile int timeout;

volatile size_t message_pointer;
uint8_t message_buffer[Packet_t::PACKET_MAX_LENGTH];
volatile Serial_states_t serial_state;

/**
 * @brief Starts the timer that runs on TA0
 *
 */
inline static void start_timeout(void);

/**
 * @brief Resets the timer, so it starts counting from 0
 *
 */
inline static void reset_timeout(void);

/**
 * @brief Disables the timer
 *
 */
inline static void stop_timeout(void);

/**
 * @brief generic handler for UART interrupts
 *
 * @param character - the character to handle
 */
inline static void uart_interrupt_handler(uint8_t character);

/**
 * @brief Configures the A0/A1 module to initialize pins 2.1/2.6 (RX) and 2.0/2.5 (TX) as UART
 */
void uart_init(void);

/**
 * @brief Writes a buffer onto the bus using A0/A1 module
 *
 * @param data - the data buffer
 * @param length - the amount of bytes that need to be sent from the buffer
 */
void uart_write(uint8_t *data, uint16_t length);

int32_t serial_close(int serial_fd)
{
    return 0;
}

int32_t serial_configure(const char *serial_port, Baudrate_t baudrate, Serial_mode_t mode, int *serial_fd)
{
    message_pointer = 0;
    serial_state = IDLE;
    BLE_message_length = 0;
    timeout_active = 0;
    timeout = DEFAULT_TIMEOUT_TICKS;
    uart_init();

    return 0;
}

int32_t serial_write(int serial_fd, char *data, size_t length)
{

    uart_write((uint8_t *)data, length);

    return 0;
}

int32_t serial_read(int serial_fd, char *data, size_t *length, int timeout_milliseconds)
{
    int return_value = -1;

    // If timeout is 0, set it to default
    // if(timeout_milliseconds <= 0)
    // {
    timeout = DEFAULT_TIMEOUT_TICKS;
    // }
    // else
    // {
    //     timeout = timeout_milliseconds * MS_TO_TICKS;
    // }
    start_timeout();
    // Waits for the serial communication to stop, or encounter a timeout
    while((serial_state == RECEIVING) || (serial_state == IDLE)) {}
    stop_timeout();

    switch(serial_state)
    {
        case PACKET_RECEIVED:
            if(!memcpy(data, message_buffer, message_pointer))
            {
                // This should never trigger in practice (added if because of MISRA)
                return_value = 1;
            }
            //            memset(message_buffer, 0, Packet_t::PACKET_MAX_LENGTH);
            *length = message_pointer;
            serial_state = IDLE;
            return_value = 0;
            break;

        case TIMEOUT:
            serial_state = IDLE;
            return_value = 2;
            break;
        case BUFFER_FULL:
            serial_state = IDLE;
            return_value = 3;
            break;
        case INVALID_DESTINATION:
            serial_state = IDLE;
            return_value = 6;
            break;
        case IDLE:
            // this should not be reached under normal conditions
            return_value = 5;
            break;
        default:
            // this should not be reached under normal conditions
            return_value = 4;
            break;
    }

    return return_value;
}

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
    serial_state = TIMEOUT;
    stop_timeout();
}

#if UART_MODULE == 0

#define RX_BIT BIT1
#define TX_BIT BIT0
#define UCAxCTLW0 UCA0CTLW0
#define UCAxBR0_local UCA0BR0
#define UCAxBR1_local UCA0BR1
#define UCAxMCTLW UCA0MCTLW
#define UCAxIE UCA0IE
#define GPIO_PORT GPIO_PORT_P3
#define GPIO_PIN GPIO_PIN4
#define UCAxIFG UCA0IFG
#define UCAxTXBUF UCA0TXBUF
#define UCAxSTATW UCA0STATW

/**
 * Setup interrupts for A0 UART module to receive messages
 */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__((interrupt(USCI_A0_VECTOR))) USCI_A0_ISR(void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(UCA0IV, USCI_UART_UCTXCPTIFG))
    {
        case USCI_NONE:
            break;
        case USCI_UART_UCRXIFG:
        {

            uint8_t character = UCA0RXBUF;

            uart_interrupt_handler(character);

            __bic_SR_register_on_exit(LPM3_bits);    // Exits low power mode

            break;
        }
        case USCI_UART_UCTXIFG:
            break;
        case USCI_UART_UCSTTIFG:
            break;
        case USCI_UART_UCTXCPTIFG:
            break;
        default:
            break;
    }
}

#elif UART_MODULE == 1

#define RX_BIT BIT6
#define TX_BIT BIT5
#define UCAxCTLW0 UCA1CTLW0
#define UCAxBR0_local UCA1BR0
#define UCAxBR1_local UCA1BR1
#define UCAxMCTLW UCA1MCTLW
#define UCAxIE UCA1IE
#define GPIO_PORT GPIO_PORT_P4
#define GPIO_PIN GPIO_PIN3
#define UCAxIFG UCA1IFG
#define UCAxTXBUF UCA1TXBUF
#define UCAxSTATW UCA1STATW

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
            break;
        case USCI_UART_UCSTTIFG:
            break;
        case USCI_UART_UCTXCPTIFG:
            break;
        default:
            break;
    }
}

#else
#error UART module not defined
#endif

void uart_init(void)
{
    // Configure GPIO
    P2SEL1 |= RX_BIT | TX_BIT;       // Sets pins 2.1 and 2.0 to function in
    P2SEL0 &= ~(RX_BIT | TX_BIT);    // secondary mode (assumed to be UART)

    // Pins start in high-impedance mode on wake-up, this enables them again
    // and activates our port configurations
    PM5CTL0 &= ~LOCKLPM5;

    // Configure USCI_A0 for UART mode with 16Mhz clock and 115200 baud rate
    UCAxCTLW0 = UCSWRST;           // Put eUSCI needs to be in reset when modifying
                                   // configurations
    UCAxCTLW0 |= UCSSEL__SMCLK;    // CLK = SMCLK, which currently is set to
                                   // 16Mhz (can be changed in future!)

    // User's Guide Table 30-5:
    // UCBRx = 8
    // UCOS16 = 1
    // UCBRFx = 10
    // UCBRSx = 0xF7
    UCAxBR0_local = 8;    // UCA0BR0 is int(16000000/(16*115200)) = 8
    UCAxBR1_local = 0x00;
    UCAxMCTLW |= UCOS16 | UCBRF_10 | 0xF700;    // UCOS16 enables over-sampling mode for high clock
                                                // speeds UCBRF_10 is int ((8.6805555555-8)*16) = 10
                                                // 0xF7 is the UCBRS modulation value

    UCAxCTLW0 &= ~UCSWRST;    // Initialize eUSCI by clearing reset

    UCAxIE |= UCRXIE;    // Enable USCI_A0 RX

    GPIO_setAsOutputPin(GPIO_PORT, GPIO_PIN);
    GPIO_setOutputLowOnPin(GPIO_PORT, GPIO_PIN);
}

void uart_write(uint8_t *data, uint16_t length)
{
    GPIO_setOutputHighOnPin(GPIO_PORT, GPIO_PIN);
    uint16_t i;
    for(i = 0; i < length; i++)
    {
        // check if TX buffer is ready.
        while(!(UCAxIFG & UCTXIFG)) {}
        UCAxTXBUF = data[i];
    }
    while((UCAxSTATW & UCBUSY) != 0) {}
    GPIO_setOutputLowOnPin(GPIO_PORT, GPIO_PIN);
}

inline static void uart_interrupt_handler(uint8_t character)
{
    // Put state in receiving mode if the state is idle and prepare for
    // receiving a message
    if(serial_state == IDLE)
    {
        message_pointer = 0;
        BLE_message_length = 0;
        // Check if its a valid start of the message, otherwise, do nothing
        if(character == Packet_t::PACKET_PREAMBLE)
        {
            serial_state = RECEIVING;
        }
    }
    // Check if state is in receiving mode to receive messages
    if(serial_state == RECEIVING)
    {

        // If timer is active, reset it, to not cause a timeout
        if(timeout_active == (uint8_t)1)
        {
            reset_timeout();
        }

        if(message_pointer >= Packet_t::PACKET_MAX_LENGTH)
        {
            serial_state = BUFFER_FULL;
        }
        else
        {
            // First 3 bytes should be PREAMBLE, SRC and DST according to BLE format, then 1 byte for packet info, and 1 byte length
            if(message_pointer < (size_t)Packet_t::PACKET_HEAD_LENGTH)
            {
                message_buffer[message_pointer] = character;
                message_pointer++;
                if((message_pointer == (size_t)3) && (character != SUBSYSTEM_ID))
                {
                    serial_state = INVALID_DESTINATION;
                }
                else if(message_pointer == (size_t)Packet_t::PACKET_HEAD_LENGTH)
                {
                    // PDU length + metadata read before + CRC length
                    BLE_message_length = character + Packet_t::PACKET_HEAD_LENGTH + Packet_t::PACKET_CRC_LENGTH;
                }
                else {}
            }
            else
            {
                // Once the first bytes have been received, rest can be copied according to the length
                if(message_pointer < ((size_t)BLE_message_length - (size_t)1))
                {
                    message_buffer[message_pointer] = character;
                    message_pointer++;
                }
                // If this is the last byte, set state to PACKET_RECEIVED
                else
                {
                    message_buffer[message_pointer] = character;
                    message_pointer++;
                    serial_state = PACKET_RECEIVED;
                }
            }
        }
    }
}
