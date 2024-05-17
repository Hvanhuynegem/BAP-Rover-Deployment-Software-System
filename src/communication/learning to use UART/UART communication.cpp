//#include "driverlib.h"
//#include <msp_serial.h>
//#include <string.h>
//
//#define UART_MODULE 0  // Set to 0 for using UCA0, or 1 for using UCA1
//
//void uart_init(void);
//void uart_write(uint8_t *data, uint16_t length);
//
//// Main function
//int main(void)
//{
//    // Stop Watchdog Timer
//    WDTCTL = WDTPW | WDTHOLD;
//
//    // Initialize UART
//    uart_init();
//
//    // Message to be transmitted
//    char message[] = "Hello, UART!";
//    size_t message_length = strlen(message);
//
//    // Main loop to repeatedly send the message
//    while (1)
//    {
//        // Send the message
//        uart_write((uint8_t *)message, message_length);
//
//        // Delay to create a visible pattern on the oscilloscope
//        __delay_cycles(16000000); // Adjust this delay as needed for visibility
//    }
//
//    return 0;
//}
//#include "communication/landerComm.h"
//#include <msp430.h>
//#include <stdint.h>
//
//#define TEST_STRING "TEST1234"
//#define BAUD_RATE 9600
//
//void delay(void) {
//    volatile unsigned int i;  // Declare as volatile to prevent optimization
//    for (i = 50000; i > 0; i--); // Arbitrary delay
//}
//
//void main(void) {
//    WDTCTL = WDTPW | WDTHOLD;  // Stop watchdog timer
//
//    // Disable the GPIO power-on default high-impedance mode
//    // to activate previously configured port settings
//    PM5CTL0 &= ~LOCKLPM5;
//
//    initialize_UART(BAUD_RATE);
//
//    // Set up pin P1.0 as output
//    P1DIR |= BIT0;             // Set P1.0 to output direction
//    P1OUT &= ~BIT0;            // Ensure LED is off initially
//
//    // Main loop
//    while (1) {
//        P1OUT ^= BIT0;         // Toggle P1.0 using exclusive-OR
//        slip_encode((uint8_t *)TEST_STRING, sizeof(TEST_STRING) - 1);
//        delay();  // Delay to allow the receiver to process
//    }
//}
