//#include <msp430.h>
//#define TX_BIT BIT5  // Define TXD as P2.5
//#define RX_BIT BIT6  // Define RXD as P2.6
//
//#define BUFFER_SIZE 128
//
//volatile unsigned char TXBuffer[BUFFER_SIZE];
//volatile unsigned int TXIndex = 0;
//volatile unsigned int TXLength = 0;
//
//void initClock(void);
//void initUART(void);
//void sendData(const unsigned char* data, unsigned int length);
//
//
//
//void initClock(void){
//    PJSEL0 |= BIT4 | BIT5;                  // Configure XT1 pins
//
//    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode to activate previously configured port settings
//
//    // XT1 Setup
//    CSCTL0_H = CSKEY >> 8;                    // Unlock CS registers
//    CSCTL1 = DCOFSEL_0;                       // Set DCO to 1MHz
//    CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK;
//    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;     // Set all dividers
//    CSCTL4 &= ~LFXTOFF;                       // Enable LFXT1
//    do
//    {
//        CSCTL5 &= ~LFXTOFFG;                    // Clear XT1 fault flag
//        SFRIFG1 &= ~OFIFG;
//    } while (SFRIFG1 & OFIFG);                // Test oscillator fault flag
//    CSCTL0_H = 0;                             // Lock CS registers
//}
//
//
//void initUART(void){
//    // Configure GPIO for UART
//    P2SEL1 |= RX_BIT | TX_BIT;        // Sets pins 2.5 and 2.6 to function in
//    P2SEL0 &= ~(RX_BIT | TX_BIT);     // secondary mode (assumed to be UART)
//
//    PM5CTL0 &= ~LOCKLPM5;             // Disable the GPIO power-on default high-impedance mode to activate previously configured port settings
//
//    // Configure USCI_A1 for UART mode
//    UCA1CTL1 |= UCSWRST;              // Put eUSCI in reset
//    UCA1CTL1 = UCSSEL__ACLK;          // Select ACLK as the clock source
//    UCA1BR0 = 3;                      // Set baud rate to 9600
//    UCA1MCTLW |= 0x5300;              // Modulation settings for 9600 baud
//    UCA1BR1 = 0;
//    UCA1CTL1 &= ~UCSWRST;             // Initialize eUSCI (release from reset)
//}
//
//void sendData(const unsigned char* data, unsigned int length){
//    while (TXLength != 0);  // Wait until previous transmission is done
//
//    // Ensure the length does not exceed buffer size
//    if (length > BUFFER_SIZE) {
//        length = BUFFER_SIZE;
//    }
//
//    // Copy data to buffer
//    for (unsigned int i = 0; i < length; i++) {
//        TXBuffer[i] = data[i];
//    }
//
//    TXLength = length;
//    TXIndex = 0;
//
//    // Trigger first transmission
//    UCA1TXBUF = TXBuffer[TXIndex++];
//    UCA1IE |= UCTXIE;  // Enable TX interrupt
//}
//
//
//int main(void)
//{
//    WDTCTL = WDTPW | WDTHOLD;  // Stop watchdog timer
//
//    initClock();
//    initUART();
//
//    __bis_SR_register(GIE);  // Enable global interrupts
//
//    // Test sendData function
//    const unsigned char testMessage[] = "Hello, UART!";
//
//    while (1) {
//        sendData(testMessage, sizeof(testMessage) - 1);
//        // Delay for 100000 cycles
//        __delay_cycles(10000);
//    }
//}
//
//
//
//
//#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
//#pragma vector = USCI_A1_VECTOR
//__interrupt void USCI_A1_ISR(void)
//#elif defined(__GNUC__)
//void __attribute__ ((interrupt(USCI_A1_VECTOR))) USCI_A1_ISR (void)
//#else
//#error Compiler not supported!
//#endif
//{
//    switch (__even_in_range(UCA1IV, 18)) {
//        case 0x00: break;  // Vector 0: No interrupts
//        case 0x02: break;  // Vector 2: UCRXIFG: The UCRXIFG interrupt flag is set each time a character is received and loaded into UCAxRXBUF.
//        case 0x04:        // Vector 4: UCTXIFG: The UCTXIFG interrupt flag is set by the transmitter to indicate that UCAxTXBUF is ready to accept another character.
//            if (TXIndex < TXLength) {
//                UCA1TXBUF = TXBuffer[TXIndex++];  // Load next byte onto buffer
//            } else {
//                TXLength = 0;  // Reset TX length to indicate completion
//                UCA1IE &= ~UCTXIE;  // Disable TX interrupt when done
//            }
//            break;
//        case 0x06:  // Vector 6: UCSTTIFG: START byte received interrupt. This flag is set when the UART module receives a START byte. This flag can be cleared by writing 0 to it.
//            UCA1IFG &= ~UCSTTIFG;
//            break;
//        case 0x08:  // Vector 8: UCTXCPTIFG: Transmit complete interrupt. This flag is set after the complete UART byte in the internal shift register including STOP bit is shifted out. This flag can be cleared by writing 0 to it.
//            UCA1IFG &= ~UCTXCPTIFG;
//            break;
//        default: break;
//    }
//}
//
